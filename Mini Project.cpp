#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //D3-SCL , D4-SDA
#include <SoftwareSerial.h>
SoftwareSerial gsm(D5, D6);
const int trig = D3;
const int echo = D4;
long duration, distance;
String data = " ";
const int buzzer = D0;
bool a = true;

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  lcd.init();
  lcd.backlight();
  delay(100);
  lcd.print("*  READY  *");
  delay(1500);
  lcd.clear();
  lcd.print("     GARBAGE    ");
  lcd.setCursor(0, 1);
  lcd.print(" MONITOR SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  ultrasonic();
  lcd.clear();
  lcd.print("LEVEL : ");
  lcd.print(data);
  lcd.setCursor(0, 1);
  lcd.print("distance : ");
  lcd.print(distance);
  delay(300);
  if (distance <= 20) {
    data = "FULL";
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.print("LEVEL : ");
    lcd.print(data);
    lcd.setCursor(0, 1);
    lcd.print("  SENDING  SMS  ");
    delay(300);
    if (a == true) {
      sms();
      a = false;
    }
    lcd.clear();
    lcd.print("LEVEL : ");
    lcd.print(data);
    lcd.setCursor(0, 1);
    lcd.print("   SMS   SENT   ");
    delay(1000);
  }
  if (distance <= 50 && distance > 20) {
    a = true;
    data = "MEDIUM";
  }
  if (distance > 50) {
    a = true;
    data = "NORMAL";
    digitalWrite(buzzer, LOW);
  }
  Serial.println(distance);
}

void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
}

void sms() {
  String message = "THE LEVEL OF GARBAGE IS HIGH : " + String(distance) + " CM \nPLEASE CLEAN IT.";
  gsm.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);
  gsm.println("AT+CMGS=\"+916281653902\"");  // Replace with your mobile number
  delay(1000);
  gsm.println(message);  // The SMS body
  delay(1000);
  gsm.write(26);  // ASCII code of CTRL+Z to send the SMS
  delay(1000);
}
