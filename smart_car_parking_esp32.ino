#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h> // LCD included

// ====== Wi-Fi Credentials ======
const char* ssid = "Mehedi";
const char* password = "11112222";

// ====== SLOT SENSORS ======
const int slot1IR = 32;    
const int slot3IR = 27;   
const int slot2Trig = 26;
const int slot2Echo = 25;

// ====== ENTRY & EXIT SENSORS ======
const int entryTrig = 33;
const int entryEcho = 14;
const int exitTrig = 19;
const int exitEcho = 18;

// ====== BUZZER & SERVO ======
const int buzzerPin = 12;
const int servoPin = 13;

// ====== OBJECTS ======
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
WebServer server(80);

// ====== THRESHOLDS ======
const int wallThreshold = 3;  
const int occupiedThreshold = 7;  
const int detectCarDistance = 15;  

// ====== VARIABLES ======
bool slot1Occupied = false;
bool slot2Occupied = false;
bool slot3Occupied = false;
int gateOpen = 0;
int parkingFull = 1;

// ====== FUNCTION TO READ ULTRASONIC ======
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  Serial.print("Distance (Trig ");
  Serial.print(trigPin);
  Serial.print(" Echo ");
  Serial.print(echoPin);
  Serial.print("): ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance == 0) return 999;
  return distance;
}

// ====== WEB PAGE ======
String getHTML() {
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='1'>";
  html += "<title>Parking Slots</title>";
  html += "<style>body{font-family:Arial;text-align:center;} table{margin:auto;border-collapse:collapse;} td,th{padding:10px;border:1px solid #ccc;} .free{color:green;} .occ{color:red;}</style>";
  html += "</head><body>";
  html += "<h1>ESP32 Parking Slot Monitor</h1>";
  html += "<table><tr><th>Slot</th><th>Status</th></tr>";
  html += "<tr><td>Slot 1</td><td class='" + String(slot1Occupied ? "occ" : "free") + "'>" + String(slot1Occupied ? "Occupied" : "Free") + "</td></tr>";
  html += "<tr><td>Slot 2</td><td class='" + String(slot2Occupied ? "occ" : "free") + "'>" + String(slot2Occupied ? "Occupied" : "Free") + "</td></tr>";
  html += "<tr><td>Slot 3</td><td class='" + String(slot3Occupied ? "occ" : "free") + "'>" + String(slot3Occupied ? "Occupied" : "Free") + "</td></tr>";
  html += "</table>";
  html += "<p>Gate: " + String(gateOpen ? "Open" : "Closed") + "</p>";
  html += "<p>Parking: " + String(parkingFull ? "Full" : "Available") + "</p>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void setup() {
  Serial.begin(115200);

  // IR slots
  pinMode(slot1IR, INPUT);
  pinMode(slot3IR, INPUT);

  // Slot 2 ultrasonic
  pinMode(slot2Trig, OUTPUT);
  pinMode(slot2Echo, INPUT);

  // Entry/Exit ultrasonic
  pinMode(entryTrig, OUTPUT);
  pinMode(entryEcho, INPUT);
  pinMode(exitTrig, OUTPUT);
  pinMode(exitEcho, INPUT);

  // Buzzer
  pinMode(buzzerPin, OUTPUT);

  // Servo
  gateServo.attach(servoPin);
  gateServo.write(0); 

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());

  // Web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // ====== SLOT STATES ======
  long slot2Distance = getDistance(slot2Trig, slot2Echo);
  slot1Occupied = (digitalRead(slot1IR) == LOW);
  slot2Occupied = (slot2Distance < occupiedThreshold);
  slot3Occupied = (digitalRead(slot3IR) == LOW);

  Serial.print("Slot 1: "); Serial.println(slot1Occupied ? "Occupied" : "Free");
  Serial.print("Slot 2: "); Serial.println(slot2Occupied ? "Occupied" : "Free");
  Serial.print("Slot 3: "); Serial.println(slot3Occupied ? "Occupied" : "Free");

  // Parking Full Check
  parkingFull = (slot1Occupied && slot2Occupied && slot3Occupied);
  Serial.println(parkingFull ? "Parking Full" : "Parking Available");

  // ====== BUZZER ======
  if (slot2Occupied && slot2Distance < wallThreshold) {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Buzzer ON: Car too close to wall");
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // ====== LCD DISPLAY ======
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1:"); lcd.print(slot1Occupied ? "X" : "O");
  lcd.print(" S2:"); lcd.print(slot2Occupied ? "X" : "O");
  lcd.print(" S3:"); lcd.print(slot3Occupied ? "X" : "O");

  lcd.setCursor(0, 1);
  if(parkingFull) {
    lcd.print("Parking Full");
  } else {
    lcd.print("Gate:"); 
    lcd.print(gateOpen ? "Open" : "Close");
  }

  // ====== ENTRY DETECTION ======
  int skipExit = 1;
  if (getDistance(entryTrig, entryEcho) < detectCarDistance && parkingFull==0) {
    Serial.println("Car at Entry - Opening Gate");
    gateServo.write(90); // Open
    gateOpen = 1;
    skipExit = 0;
  } else if (gateOpen) {
    gateOpen = 0;
    delay(3000);
    gateServo.write(0); // Close
  }

  // ====== EXIT DETECTION ======
  if (getDistance(exitTrig, exitEcho) < detectCarDistance) {
    Serial.println("Car at Exit - Opening Gate");
    gateServo.write(90); // Open
    gateOpen = 1;
  } else if (gateOpen & skipExit) {
    gateOpen = 0;
    delay(3000);
    gateServo.write(0); // Close
  }

  // Handle web requests
  server.handleClient();

  Serial.println("------------------------");
  delay(500);
}
