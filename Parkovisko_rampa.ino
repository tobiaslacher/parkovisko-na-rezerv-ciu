include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// ===== WIFI =====
const char* ssid = "-123";
const char* password = "zabudni123";

// ===== PINY =====
#define IR1_PIN 23
#define IR2_PIN 19
#define SERVO_PIN 17

// ===== SERVO =====
Servo gateServo;
const int GATE_OPEN = 90;
const int GATE_CLOSED = 0;

// ===== PARKOVISKO =====
#define MAX_CARS 5
int freeSpots = MAX_CARS;

// ===== OLED =====
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// ===== RAMPA =====
unsigned long gateTimer = 0;
const unsigned long GATE_OPEN_TIME = 2000;
bool gateOpen = false;

// ===== WEB SERVER =====
WiFiServer server(80);

// ===== IR STAVY =====
bool lastIR1 = HIGH;
bool lastIR2 = HIGH;

// =============================

void openGate() {
gateServo.write(GATE_OPEN);
gateTimer = millis();
gateOpen = true;

Serial.println("RAMPA OTVORENA");
}

void closeGate() {
gateServo.write(GATE_CLOSED);
gateOpen = false;

Serial.println("RAMPA ZATVORENA");
}

void updateDisplay() {
display.clearDisplay();

display.setTextSize(2);
display.setTextColor(SSD1306_WHITE);

display.setCursor(0, 8);
display.print("FREE:");
display.print(freeSpots);

display.display();
}

String getHTML() {
String html = "";
html += "";
html += "";

html += "";

html += "";

html += "

🅿️ PARKOVISKO LIVE
";
html += "

Voľné miesta: ";
html += String(freeSpots);
html += " / ";
html += String(MAX_CARS);
html += "
";
html += "

Obsadené: ";
html += String(MAX_CARS - freeSpots);
html += "
";
html += "

Rampa: ";
html += gateOpen ? "🟢 OTVORENÁ" : "🔴 ZATVORENÁ";
html += "
";
html += "";

return html;
}

// =============================

void setup() {

Serial.begin(115200);

// ===== IR SENZORY =====
pinMode(IR1_PIN, INPUT);
pinMode(IR2_PIN, INPUT);

// ===== SERVO =====
gateServo.setPeriodHertz(50);
gateServo.attach(SERVO_PIN, 500, 2400);

closeGate();

// ===== OLED =====
Wire.begin(27, 25);

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

updateDisplay();

// ===== WIFI =====
Serial.println();
Serial.println("Pripajam na WiFi...");

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println();
Serial.println("WiFi PRIPOJENE!");
Serial.print("IP: http://");
Serial.println(WiFi.localIP());

// ===== SERVER =====
server.begin();
}

// =============================

void loop() {

int ir1 = digitalRead(IR1_PIN);
int ir2 = digitalRead(IR2_PIN);

// DEBUG
Serial.print("IR1: ");
Serial.print(ir1);

Serial.print(" | IR2: ");
Serial.println(ir2);

// =========================
// AUTO VCHADZA
// =========================
if (!gateOpen &&
lastIR1 == HIGH &&
ir1 == LOW &&
freeSpots > 0) {

openGate();  

freeSpots--;  

updateDisplay();  

Serial.println("AUTO VOSLO");  
Serial.print("VOLNE MIESTA: ");  
Serial.println(freeSpots);  

delay(500);  
}

// =========================
// AUTO ODCHADZA
// =========================
if (!gateOpen &&
lastIR2 == HIGH &&
ir2 == LOW &&
freeSpots < MAX_CARS) {

openGate();  

freeSpots++;  

updateDisplay();  

Serial.println("AUTO ODISLO");  
Serial.print("VOLNE MIESTA: ");  
Serial.println(freeSpots);  

delay(500);  
}

// ULOZENIE POSLEDNEHO STAVU
lastIR1 = ir1;
lastIR2 = ir2;

// =========================
// ZATVORENIE RAMPY
// =========================
if (gateOpen &&
millis() - gateTimer >= GATE_OPEN_TIME) {

closeGate();  
}

// =========================
// WEB SERVER
// =========================
WiFiClient client = server.available();

if (client) {

while (client.connected() && !client.available()) {  
  delay(1);  
}  

client.readStringUntil('\r');  

client.println("HTTP/1.1 200 OK");  
client.println("Content-type:text/html");  
client.println("Connection: close");  
client.println();  

client.print(getHTML());  

client.stop();  
}
} 
