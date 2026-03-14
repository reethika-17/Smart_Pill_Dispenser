#include <Servo.h>
#include <LiquidCrystal.h>

Servo motor;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int mPin = 6;
const int bPin = 9;
const int ledPin = 7;
const int bzPin = 8;

unsigned long lastSecondUpdate = 0;
unsigned long pillTimerStart = 0;

int hour = 0, min = 0, sec = 0;

int angles[4] = {0, 45, 90, 135};
int currentPill = 0;

bool waitingForButton = false;
bool timerRunning = false;

unsigned long dispenseInterval = 5000;  

void setup() {
  pinMode(bPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(bzPin, OUTPUT);

  motor.attach(mPin);
  motor.write(angles[0]);

  lcd.begin(16, 2);
}

void loop() {
  updateFakeTime();
  updateLCD();

  if (currentPill >= 4) {
    lcd.setCursor(0, 0);
    lcd.print("All pills taken ");
    return;
  }

  if (!timerRunning && !waitingForButton) {
    pillTimerStart = millis();
    timerRunning = true;
  }

  if (timerRunning && millis() - pillTimerStart >= dispenseInterval) {
    dispensePill();
    timerRunning = false;
    waitingForButton = true;
  }

  if (waitingForButton && digitalRead(bPin) == HIGH) {
    digitalWrite(ledPin, LOW);
    digitalWrite(bzPin, LOW);
    lcd.clear();
    lcd.print("Pill taken");

    delay(1000);

    waitingForButton = false;
    currentPill++;
  }
}


void dispensePill() {
  motor.write(angles[currentPill]);

  lcd.clear();
  lcd.print("Take pill ");
  lcd.print(currentPill + 1);

  digitalWrite(ledPin, HIGH);
  digitalWrite(bzPin, HIGH);
}

void updateFakeTime() {
  if (millis() - lastSecondUpdate >= 1000) {
    lastSecondUpdate = millis();
    sec++;

    if (sec >= 60) {
      sec = 0;
      min++;
    }
    if (min >= 60) {
      min = 0;
      hour++;
    }
    if (hour >= 24) {
      hour = 0;
    }
  }
}

void updateLCD() {
  lcd.setCursor(0, 1);
  if (hour < 10) lcd.print("0");
  lcd.print(hour);
  lcd.print(":");
  if (min < 10) lcd.print("0");
  lcd.print(min);
  lcd.print(":");
  if (sec < 10) lcd.print("0");
  lcd.print(sec);
}