#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C (địa chỉ thường là 0x27 hoặc 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo (vòi nước)
Servo servoPipe;

// Pin
#define SOIL_PIN A0
#define RAIN_PIN A2
#define RELAY_PIN 7
#define LED_PIN 13

int soilValue, rainValue;

void setup() {
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // tắt bơm
  digitalWrite(LED_PIN, LOW);

  servoPipe.attach(9);
  servoPipe.write(0);

  // LCD I2C
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Smart Garden");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Đọc cảm biến
  soilValue = analogRead(SOIL_PIN);
  rainValue = analogRead(RAIN_PIN);

  // In Serial
  Serial.print("DAT AM: ");
  Serial.print(soilValue);
  Serial.print(" | MUA: ");
  Serial.println(rainValue);

  // LCD dòng 1
  lcd.setCursor(0, 0);
  lcd.print("D:");
  lcd.print(soilValue);
  lcd.print(" M:");
  lcd.print(rainValue);
  lcd.print("   ");

  // LCD dòng 2
  lcd.setCursor(0, 1);

  // Nếu mưa
  if (rainValue < 750) {
    digitalWrite(RELAY_PIN, LOW); 
    digitalWrite(LED_PIN, LOW);

    servoPipe.write(120);

    lcd.print("MUA - HUNG NUOC ");
  } 
  else {
    servoPipe.write(0); 

    // Đất khô
    if (soilValue > 600) {
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);

      lcd.print("DAT KHO - TUOI  ");
    } 
    else {
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);

      lcd.print("DAT DU NUOC     ");
    }
  }

  delay(1000);
}