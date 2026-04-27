#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ---------- RFID ----------
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// ---------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- SERVO ----------
Servo gate;
int servoPin = 7;

// ---------- IR SENSORS ----------
int s1 = 2;
int s2 = 3;
int s3 = 4;
int s4 = 5;
int s5 = 6;

int exitIR = 8;

// ---------- RFID CARDS ----------
String cards[] = {
  "D3 F5 D7 12",
  "63 25 C5 11",
  "53 C1 32 11",
  "53 4B 18 11"
};

// ---------- ACTIVE SLOT ----------
String activeSlot = "";

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();

  gate.attach(servoPin);
  gate.write(0);

  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);

  pinMode(exitIR, INPUT);

  lcd.setCursor(0,0);
  lcd.print("Smart Parking");
  lcd.setCursor(0,1);
  lcd.print("Scan RFID");
}

// ---------- LOOP ----------
void loop() {
  checkRFID();
  checkExit();
  showSlots();
}

// ---------- RFID ENTRY ----------
void checkRFID() {

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    String uid = getUID();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RFID Scan");

    if (isAllowed(uid)) {

      activeSlot = getFreeSlot();

      if (activeSlot != "NONE") {

        // ---- SEND ENTRY TO PYTHON ----
        Serial.print("ENTRY,");
        Serial.print(activeSlot);
        Serial.print(",");
        Serial.println(uid);

        lcd.setCursor(0,1);
        lcd.print("Access OK");

        openGate();
      }
      else {
        Serial.print("FULL,");
        Serial.println(uid);

        lcd.setCursor(0,1);
        lcd.print("Parking Full");
      }

    } else {

      Serial.print("DENIED,");
      Serial.println(uid);

      lcd.setCursor(0,1);
      lcd.print("Denied");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(800);
  }
}

// ---------- EXIT ----------
void checkExit() {

  if (digitalRead(exitIR) == LOW && activeSlot != "") {

    // ---- SEND EXIT TO PYTHON ----
    Serial.print("EXIT,");
    Serial.println(activeSlot);

    lcd.clear();
    lcd.print("Exit Vehicle");

    openGate();

    activeSlot = "";
    delay(1000);
  }
}

// ---------- FREE SLOT ----------
String getFreeSlot() {

  if (digitalRead(s1) == HIGH) return "S1";
  if (digitalRead(s2) == HIGH) return "S2";
  if (digitalRead(s3) == HIGH) return "S3";
  if (digitalRead(s4) == HIGH) return "S4";
  if (digitalRead(s5) == HIGH) return "S5";

  return "NONE";
}

// ---------- UID ----------
String getUID() {

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
    uid.toUpperCase();
    uid += " ";
  }

  return uid;
}

// ---------- AUTH ----------
bool isAllowed(String uid) {

  for (int i = 0; i < 4; i++) {
    if (uid.indexOf(cards[i]) >= 0) return true;
  }
  return false;
}

// ---------- GATE ----------
void openGate() {
  gate.write(90);
  delay(3000);
  gate.write(0);
}

// ---------- LCD ----------
void showSlots() {

  lcd.setCursor(0,0);
  lcd.print("S1 S2 S3 S4 S5");

  lcd.setCursor(0,1);

  lcd.print(digitalRead(s1) == LOW ? "F  " : "E  ");
  lcd.print(digitalRead(s2) == LOW ? "F  " : "E  ");
  lcd.print(digitalRead(s3) == LOW ? "F  " : "E  ");
  lcd.print(digitalRead(s4) == LOW ? "F  " : "E  ");
  lcd.print(digitalRead(s5) == LOW ? "F  " : "E  ");
}
