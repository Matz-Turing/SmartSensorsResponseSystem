#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 3
#define TEMP_SENSOR A0
#define BUZZER_PIN 8
#define MOTOR_PIN 11
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 7

Servo servoMotor;
Adafruit_LiquidCrystal lcd_1(0);
Adafruit_LiquidCrystal lcd_2(1);

float readTemperature() {
  int analogValue = analogRead(TEMP_SENSOR);
  float voltage = analogValue * (5.0 / 1023.0);
  return (voltage - 0.5) * 100.0;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(0);

  lcd_1.begin(16, 2);
  lcd_2.begin(16, 2);
  lcd_1.print("Iniciando...");
  lcd_2.print("Sistema Ativo");
  Serial.begin(9600);
}

void loop() {
  long duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Distancia: ");
  lcd_1.print(distance);
  lcd_1.print(" cm");

  if (distance < 10) {
    lcd_1.setCursor(0, 1);
    lcd_1.print("Servo: ON ");
    servoMotor.write(90);
  } else {
    lcd_1.setCursor(0, 1);
    lcd_1.print("Servo: OFF");
    servoMotor.write(0);
  }

  float temperature = readTemperature();
  lcd_2.clear();
  lcd_2.setCursor(0, 0);
  lcd_2.print("Temp: ");
  lcd_2.print(temperature);
  lcd_2.print(" C");

  if (temperature > 30.0) {
    digitalWrite(MOTOR_PIN, HIGH);
    tone(BUZZER_PIN, 1000, 500);
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
  }

  delay(500);
}