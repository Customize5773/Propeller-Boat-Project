/*
 * RC Boat with RCSwitch Library - 1 Motor Version
 * 
 * Install required library:
 * Sketch -> Include Library -> Manage Libraries -> Search "RCSwitch"
 * 
 * Hardware:
 * - 1 DC Motor for propulsion
 * - 1 Servo for steering
 * - 4 Ultrasonic sensors
 * - RF 433MHz receiver
 */

#include <RCSwitch.h>
#include <Servo.h>

// Motor Driver Pins
#define MOTOR_FWD 5
#define MOTOR_BWD 6

// Servo Pin
#define SERVO_PIN 9

// Ultrasonic Sensor Pins
#define TRIG_FWD 2
#define ECHO_FWD 3
#define TRIG_RGT 4
#define ECHO_RGT 7
#define TRIG_LFT 8
#define ECHO_LFT 11
#define TRIG_BWD 12
#define ECHO_BWD 13

// RF Receiver (use interrupt pin)
#define RF_RECEIVER 0  // Interrupt 0 = Pin 2 on most Arduinos
                       // If using Pin 2 for ultrasonic, use Pin 3 (interrupt 1)

// Constants
#define SAFE_DISTANCE 30
#define MOTOR_SPEED 200
#define MOTOR_SLOW 120

// Servo positions
#define SERVO_CENTER 90
#define SERVO_LEFT 45
#define SERVO_RIGHT 135
#define SERVO_SLIGHT_LEFT 70
#define SERVO_SLIGHT_RIGHT 110

// Objects
RCSwitch mySwitch = RCSwitch();
Servo rudderServo;

// Variables
int distanceFwd, distanceRgt, distanceLft, distanceBwd;
bool obstacleAvoidance = true;
unsigned long lastCommandTime = 0;
#define COMMAND_TIMEOUT 500

void setup() {
  Serial.begin(9600);
  
  // Motor pins
  pinMode(MOTOR_FWD, OUTPUT);
  pinMode(MOTOR_BWD, OUTPUT);
  
  // Servo
  rudderServo.attach(SERVO_PIN);
  rudderServo.write(SERVO_CENTER);
  
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
  
  stopMotor();
  Serial.println("RC Boat (1 Motor) Ready!");
  Serial.println("Press remote buttons and note the codes in Serial Monitor");
}

void loop() {
  // Update sensor readings
  updateSensors();
  
  // Check for RF commands
  if (mySwitch.available()) {
    unsigned long receivedCode = mySwitch.getReceivedValue();
    
    if (receivedCode != 0) {
      Serial.print("Received code: ");
      Serial.println(receivedCode);
      processCommand(receivedCode);
      lastCommandTime = millis();
    }
    
    mySwitch.resetAvailable();
  }
  
  // Safety timeout - stop if no command received
  if (millis() - lastCommandTime > COMMAND_TIMEOUT) {
    stopMotor();
    centerRudder();
  }
  
  delay(50);
}

void updateSensors() {
  distanceFwd = getDistance(TRIG_FWD, ECHO_FWD);
  distanceRgt = getDistance(TRIG_RGT, ECHO_RGT);
  distanceLft = getDistance(TRIG_LFT, ECHO_LFT);
  distanceBwd = getDistance(TRIG_BWD, ECHO_BWD);
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
  /*
   * IMPORTANT: Replace these codes with YOUR remote's codes!
   * 
   * To find your codes:
   * 1. Upload this code
   * 2. Open Serial Monitor (9600 baud)
   * 3. Press each button on your remote
   * 4. Note the code numbers
   * 5. Replace the numbers below
   */
  
  switch(code) {
    case 1234567:  // Forward - REPLACE WITH YOUR CODE
      if (isSafe(distanceFwd)) {
        centerRudder();
        moveForward(MOTOR_SPEED);
        Serial.println("→ Forward");
      } else {
        stopMotor();
        Serial.println("⚠ Obstacle ahead!");
      }
      break;
      
    case 1234568:  // Backward - REPLACE WITH YOUR CODE
      if (isSafe(distanceBwd)) {
        centerRudder();
        moveBackward(MOTOR_SPEED);
        Serial.println("← Backward");
      } else {
        stopMotor();
        Serial.println("⚠ Obstacle behind!");
      }
      break;
      
    case 1234569:  // Left - REPLACE WITH YOUR CODE
      turnLeft();
      Serial.println("↶ Turn Left");
      break;
      
    case 1234570:  // Right - REPLACE WITH YOUR CODE
      turnRight();
      Serial.println("↷ Turn Right");
      break;
      
    case 1234571:  // Forward-Left - REPLACE WITH YOUR CODE
      if (isSafe(distanceFwd)) {
        rudderServo.write(SERVO_SLIGHT_LEFT);
        moveForward(MOTOR_SPEED);
        Serial.println("↖ Forward Left");
      }
      break;
      
    case 1234572:  // Forward-Right - REPLACE WITH YOUR CODE
      if (isSafe(distanceFwd)) {
        rudderServo.write(SERVO_SLIGHT_RIGHT);
        moveForward(MOTOR_SPEED);
        Serial.println("↗ Forward Right");
      }
      break;
      
    case 1234573:  // Stop - REPLACE WITH YOUR CODE
      stopMotor();
      centerRudder();
      Serial.println("■ Stop");
      break;
      
    case 1234574:  // Toggle Obstacle Avoidance - REPLACE WITH YOUR CODE
      obstacleAvoidance = !obstacleAvoidance;
      Serial.print("Obstacle Avoidance: ");
      Serial.println(obstacleAvoidance ? "ON" : "OFF");
      break;
      
    default:
      Serial.print("Unknown code: ");
      Serial.println(code);
      break;
  }
}

bool isSafe(int distance) {
  if (!obstacleAvoidance) return true;
  return distance > SAFE_DISTANCE;
}

void moveForward(int speed) {
  analogWrite(MOTOR_FWD, speed);
  analogWrite(MOTOR_BWD, 0);
}

void moveBackward(int speed) {
  analogWrite(MOTOR_FWD, 0);
  analogWrite(MOTOR_BWD, speed);
}

void stopMotor() {
  analogWrite(MOTOR_FWD, 0);
  analogWrite(MOTOR_BWD, 0);
}

void centerRudder() {
  rudderServo.write(SERVO_CENTER);
}

void turnLeft() {
  rudderServo.write(SERVO_LEFT);
  moveForward(MOTOR_SLOW);
}

void turnRight() {
  rudderServo.write(SERVO_RIGHT);
  moveForward(MOTOR_SLOW);
}

/* 
 * Optional: Add this to loop() to print sensor data when you send 'd' via Serial Monitor
 * 
 * if (Serial.available() > 0 && Serial.read() == 'd') {
 *   Serial.print("F:");
 *   Serial.print(distanceFwd);
 *   Serial.print(" R:");
 *   Serial.print(distanceRgt);
 *   Serial.print(" L:");
 *   Serial.print(distanceLft);
 *   Serial.print(" B:");
 *   Serial.println(distanceBwd);
 * }
 */
