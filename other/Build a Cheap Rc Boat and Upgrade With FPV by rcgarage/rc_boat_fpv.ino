/*
 * Cheap RC Boat with FPV Upgrade
 * 
 * This code controls a basic RC boat using:
 * - Arduino Nano/Uno
 * - NRF24L01 radio module for control
 * - L298N motor driver
 * - 2x DC motors (or brushless ESCs)
 * - Servo for rudder (optional)
 * - FPV camera support
 * 
 * Author: Claude
 * Date: 2026
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Pin definitions
#define CE_PIN 9
#define CSN_PIN 10
#define MOTOR_LEFT_PWM 5
#define MOTOR_LEFT_DIR1 4
#define MOTOR_LEFT_DIR2 3
#define MOTOR_RIGHT_PWM 6
#define MOTOR_RIGHT_DIR1 7
#define MOTOR_RIGHT_DIR2 8
#define RUDDER_SERVO_PIN 2
#define LED_PIN 13
#define VOLTAGE_SENSOR A0

// Radio setup
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Control structure
struct ControlData {
  int throttle;    // -255 to 255
  int steering;    // -255 to 255
  bool lights;     // LED control
  bool fpvRecord;  // FPV camera recording trigger
};

ControlData receivedData;
Servo rudderServo;

// Safety and status
unsigned long lastReceiveTime = 0;
const unsigned long FAILSAFE_TIMEOUT = 1000; // 1 second
bool connectionLost = false;

// Battery monitoring
float batteryVoltage = 0;
const float LOW_VOLTAGE_CUTOFF = 6.5; // Adjust based on your battery

void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_DIR1, OUTPUT);
  pinMode(MOTOR_LEFT_DIR2, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR1, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(VOLTAGE_SENSOR, INPUT);
  
  // Initialize servo
  rudderServo.attach(RUDDER_SERVO_PIN);
  rudderServo.write(90); // Center position
  
  // Initialize radio
  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(100);
    }
  }
  
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  
  Serial.println("RC Boat Initialized");
  
  // Startup sequence
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void loop() {
  // Check for incoming radio data
  if (radio.available()) {
    radio.read(&receivedData, sizeof(ControlData));
    lastReceiveTime = millis();
    connectionLost = false;
  }
  
  // Failsafe check
  if (millis() - lastReceiveTime > FAILSAFE_TIMEOUT) {
    if (!connectionLost) {
      Serial.println("Connection lost - engaging failsafe");
      connectionLost = true;
    }
    stopMotors();
    rudderServo.write(90);
    // Blink LED to indicate connection loss
    digitalWrite(LED_PIN, (millis() / 500) % 2);
    return;
  }
  
  // Battery monitoring
  checkBattery();
  
  // Control motors based on received data
  controlMotors(receivedData.throttle, receivedData.steering);
  
  // Control rudder servo
  int rudderAngle = map(receivedData.steering, -255, 255, 45, 135);
  rudderServo.write(rudderAngle);
  
  // LED control
  digitalWrite(LED_PIN, receivedData.lights);
  
  // Debug output
  if (millis() % 500 < 10) {
    Serial.print("Throttle: ");
    Serial.print(receivedData.throttle);
    Serial.print(" | Steering: ");
    Serial.print(receivedData.steering);
    Serial.print(" | Battery: ");
    Serial.print(batteryVoltage);
    Serial.println("V");
  }
}

void controlMotors(int throttle, int steering) {
  // Mix throttle and steering for differential drive
  int leftMotor = throttle + steering / 2;
  int rightMotor = throttle - steering / 2;
  
  // Constrain values
  leftMotor = constrain(leftMotor, -255, 255);
  rightMotor = constrain(rightMotor, -255, 255);
  
  // Apply to left motor
  if (leftMotor > 0) {
    digitalWrite(MOTOR_LEFT_DIR1, HIGH);
    digitalWrite(MOTOR_LEFT_DIR2, LOW);
    analogWrite(MOTOR_LEFT_PWM, leftMotor);
  } else if (leftMotor < 0) {
    digitalWrite(MOTOR_LEFT_DIR1, LOW);
    digitalWrite(MOTOR_LEFT_DIR2, HIGH);
    analogWrite(MOTOR_LEFT_PWM, abs(leftMotor));
  } else {
    digitalWrite(MOTOR_LEFT_DIR1, LOW);
    digitalWrite(MOTOR_LEFT_DIR2, LOW);
    analogWrite(MOTOR_LEFT_PWM, 0);
  }
  
  // Apply to right motor
  if (rightMotor > 0) {
    digitalWrite(MOTOR_RIGHT_DIR1, HIGH);
    digitalWrite(MOTOR_RIGHT_DIR2, LOW);
    analogWrite(MOTOR_RIGHT_PWM, rightMotor);
  } else if (rightMotor < 0) {
    digitalWrite(MOTOR_RIGHT_DIR1, LOW);
    digitalWrite(MOTOR_RIGHT_DIR2, HIGH);
    analogWrite(MOTOR_RIGHT_PWM, abs(rightMotor));
  } else {
    digitalWrite(MOTOR_RIGHT_DIR1, LOW);
    digitalWrite(MOTOR_RIGHT_DIR2, LOW);
    analogWrite(MOTOR_RIGHT_PWM, 0);
  }
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_DIR1, LOW);
  digitalWrite(MOTOR_LEFT_DIR2, LOW);
  analogWrite(MOTOR_LEFT_PWM, 0);
  digitalWrite(MOTOR_RIGHT_DIR1, LOW);
  digitalWrite(MOTOR_RIGHT_DIR2, LOW);
  analogWrite(MOTOR_RIGHT_PWM, 0);
}

void checkBattery() {
  // Read voltage (adjust voltage divider ratio as needed)
  int rawValue = analogRead(VOLTAGE_SENSOR);
  batteryVoltage = (rawValue / 1024.0) * 5.0 * 3.0; // Assuming 3:1 voltage divider
  
  // Low voltage cutoff
  if (batteryVoltage < LOW_VOLTAGE_CUTOFF && batteryVoltage > 1.0) {
    stopMotors();
    // Rapid blink to indicate low battery
    digitalWrite(LED_PIN, (millis() / 100) % 2);
  }
}
