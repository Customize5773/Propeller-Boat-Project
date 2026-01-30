/*
 * RC Boat with RCSwitch Library - Simplified Version
 * 
 * Install required library:
 * Sketch -> Include Library -> Manage Libraries -> Search "RCSwitch"
 * 
 * This version uses the RCSwitch library for better RF control
 */

#include <RCSwitch.h>

// Motor Driver Pins
#define MOTOR_LEFT_FWD 5
#define MOTOR_LEFT_BWD 6
#define MOTOR_RIGHT_FWD 9
#define MOTOR_RIGHT_BWD 10

// Ultrasonic Sensor Pins
#define TRIG_FWD 2
#define ECHO_FWD 3
#define TRIG_RGT 4
#define ECHO_RGT 7
#define TRIG_LFT 8
#define ECHO_LFT 11
#define TRIG_BWD 12
#define ECHO_BWD 13

// RF Receiver
#define RF_RECEIVER 0  // Interrupt pin (D2 on most Arduinos)

// Constants
#define SAFE_DISTANCE 30
#define MOTOR_SPEED 200

// RCSwitch object
RCSwitch mySwitch = RCSwitch();

// Variables
int distanceFwd, distanceRgt, distanceLft, distanceBwd;
bool obstacleAvoidance = true;
unsigned long lastCommandTime = 0;
#define COMMAND_TIMEOUT 500  // Stop if no command for 500ms

void setup() {
  Serial.begin(9600);
  
  // Motor pins
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);
  
  // Ultrasonic pins
  pinMode(TRIG_FWD, OUTPUT);
  pinMode(ECHO_FWD, INPUT);
  pinMode(TRIG_RGT, OUTPUT);
  pinMode(ECHO_RGT, INPUT);
  pinMode(TRIG_LFT, OUTPUT);
  pinMode(ECHO_LFT, INPUT);
  pinMode(TRIG_BWD, OUTPUT);
  pinMode(ECHO_BWD, INPUT);
  
  // RF Receiver
  mySwitch.enableReceive(RF_RECEIVER);
  
  stopMotors();
  Serial.println("RC Boat Ready!");
}

void loop() {
  // Update sensor readings
  updateSensors();
  
  // Check for RF commands
  if (mySwitch.available()) {
    unsigned long receivedCode = mySwitch.getReceivedValue();
    
    if (receivedCode != 0) {
      processCommand(receivedCode);
      lastCommandTime = millis();
    }
    
    mySwitch.resetAvailable();
  }
  
  // Safety timeout - stop if no command received
  if (millis() - lastCommandTime > COMMAND_TIMEOUT) {
    stopMotors();
  }
  
  delay(50);
}

void updateSensors() {
  distanceFwd = getDistance(TRIG_FWD, ECHO_FWD);
  distanceRgt = getDistance(TRIG_RGT, ECHO_RGT);
  distanceLft = getDistance(TRIG_LFT, ECHO_LFT);
  distanceBwd = getDistance(TRIG_BWD, ECHO_BWD);
  
  // Print sensor data
  if (Serial.available() > 0 && Serial.read() == 'd') {
    Serial.print("F:");
    Serial.print(distanceFwd);
    Serial.print(" R:");
    Serial.print(distanceRgt);
    Serial.print(" L:");
    Serial.print(distanceLft);
    Serial.print(" B:");
    Serial.println(distanceBwd);
  }
}

int getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duration = pulseIn(echo, HIGH, 30000);
  if (duration == 0) return 999;
  
  return duration * 0.034 / 2;
}

void processCommand(unsigned long code) {
  Serial.print("Command: ");
  Serial.println(code);
  
  // Map your remote's button codes here
  // Use Serial Monitor to identify your remote's codes
  switch(code) {
    case 1234567:  // Forward button (replace with your code)
      if (isSafe(distanceFwd)) moveForward();
      else stopMotors();
      break;
      
    case 1234568:  // Backward
      if (isSafe(distanceBwd)) moveBackward();
      else stopMotors();
      break;
      
    case 1234569:  // Left
      turnLeft();
      break;
      
    case 1234570:  // Right
      turnRight();
      break;
      
    case 1234571:  // Stop
      stopMotors();
      break;
      
    default:
      Serial.println("Unknown command");
      break;
  }
}

bool isSafe(int distance) {
  if (!obstacleAvoidance) return true;
  return distance > SAFE_DISTANCE;
}

void moveForward() {
  analogWrite(MOTOR_LEFT_FWD, MOTOR_SPEED);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}

void moveBackward() {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, MOTOR_SPEED);
}

void turnLeft() {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, MOTOR_SPEED * 0.7);
  analogWrite(MOTOR_RIGHT_FWD, MOTOR_SPEED * 0.7);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}

void turnRight() {
  analogWrite(MOTOR_LEFT_FWD, MOTOR_SPEED * 0.7);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, MOTOR_SPEED * 0.7);
}

void stopMotors() {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}
