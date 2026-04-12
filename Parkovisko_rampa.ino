#define BLYNK_TEMPLATE_ID "TMPL4BjFDmEwU"
#define BLYNK_TEMPLATE_NAME "Parkovacia Rampa Datadraci"
#define BLYNK_AUTH_TOKEN "blHKC9pU92kRuBEOJEZ3_WET14FFPz99"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// ---------- PINY ----------
#define IR1_PIN 23   // vjazd
#define IR2_PIN 19   // výstup
#define SERVO_PIN 17

Servo gateServo;
const int GATE_OPEN = 90;
const int GATE_CLOSED = 0;

#define MAX_CARS 5
int freeSpots = MAX_CARS;

// OLED
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// ---------- TIMER ----------
unsigned long gateTimer = 0;
const unsigned long GATE_OPEN_TIME = 2000;

bool gateOpen = false;

// WiFi údaje - ZMEŇ NA SVOJE
char ssid[] = "Sem napíš presné meno tvojej WiFi";      // ← sem napíš meno WiFi
char pass[] = "Sem napíš heslo k tvojej WiFi";     // ← sem napíš heslo WiFi

BlynkTimer timer;

// ---------- FUNKCIE ----------
void openGate() {
  gateServo.write(GATE_OPEN);
  gateTimer = millis();
  gateOpen = true;
  Blynk.virtualWrite(V1, "OPEN");
  Serial.println("Rampa otvorena");
}

void closeGate() {
  gateServo.write(GATE_CLOSED);
  gateOpen = false;
  Blynk.virtualWrite(V1, "CLOSED");
  Serial.println("Rampa zatvorena");
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 8);
  display.print("FREE: ");
  display.print(freeSpots);
  display.display();
}

void sendDataToBlynk() {
  Blynk.virtualWrite(V0, freeSpots);           // voľné miesta
  Blynk.virtualWrite(V2, MAX_CARS - freeSpots); // autá vnútri
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);

  gateServo.setPeriodHertz(50);
  gateServo.attach(SERVO_PIN, 500, 2400);
  closeGate();

  Wire.begin(27, 25);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED chyba");
    while (1);
  }
  updateDisplay();

  // Pripojenie k WiFi a Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendDataToBlynk);
}

// ---------- LOOP ----------
void loop() {
  Blynk.run();
  timer.run();

  int ir1 = digitalRead(IR1_PIN);
  int ir2 = digitalRead(IR2_PIN);

  // VJAZD
  if (!gateOpen && ir1 == LOW && freeSpots > 0) {
    openGate();
    freeSpots--;
    updateDisplay();
    sendDataToBlynk();
  }

  // VÝJAZD
  if (!gateOpen && ir2 == LOW && freeSpots < MAX_CARS) {
    openGate();
    freeSpots++;
    updateDisplay();
    sendDataToBlynk();
  }

  // AUTOMATICKÉ ZATVORENIE RAMPY
  if (gateOpen && (millis() - gateTimer >= GATE_OPEN_TIME)) {
    closeGate();
  }
}
