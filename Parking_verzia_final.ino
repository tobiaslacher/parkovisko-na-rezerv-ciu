#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// ---------- PINY ----------
#define IR1_PIN 23   // vjazd
#define IR2_PIN 19   // vystup
#define SERVO_PIN 17

Servo gateServo;
const int GATE_OPEN = 90;
const int GATE_CLOSED = 0;

#define MAX_CARS 5
int freeSpots = MAX_CARS;

// OLED
Adafruit_SSD1306 display(128, 32, &Wire, -1);

// ---------- TIMER ----------
unsigned long gateTimer = 0;               // cas otvorenia brány
const unsigned long GATE_OPEN_TIME = 2000; // 2 sekundy

// ---------- STAVY ----------
bool gateOpen = false; // je brána otvorená

// ---------- FUNKCIE ----------
void openGate() {
  gateServo.write(GATE_OPEN);
  gateTimer = millis();
  gateOpen = true;
  Serial.println("Rampa otvorena");
}

void closeGate() {
  gateServo.write(GATE_CLOSED);
  gateOpen = false;
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

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);

  // Servo ESP32
  gateServo.setPeriodHertz(50);
  gateServo.attach(SERVO_PIN, 500, 2400);
  closeGate();

  // OLED
  Wire.begin(27, 25);    // SDA, SCL
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED chyba");
    while (1);
  }
  updateDisplay();
}

// ---------- LOOP ----------
void loop() {
  int ir1 = digitalRead(IR1_PIN);
  int ir2 = digitalRead(IR2_PIN);

  // -------- VJAZD --------
  if (!gateOpen && ir1 == LOW && freeSpots > 0) {
    openGate();
    freeSpots--;
    updateDisplay();
  }

  // -------- VÝJAZD --------
  if (!gateOpen && ir2 == LOW && freeSpots < MAX_CARS) {
    openGate();
    freeSpots++;
    updateDisplay();
  }

  // -------- ZATVORENIE PO ČASE --------
  if (gateOpen && (millis() - gateTimer >= GATE_OPEN_TIME)) {
    closeGate();
  }
}
