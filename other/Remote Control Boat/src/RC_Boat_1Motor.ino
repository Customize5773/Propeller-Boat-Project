/*
 * Remote Control Boat with 1 Motor + Servo Steering and 4 Ultrasonic Sensors
 * 
 * Features:
 * - 1 DC motor for propulsion (forward/backward)
 * - 1 Servo motor for steering (left/right)
 * - 4 ultrasonic sensors (Forward, Right, Left, Backward)
 * - Obstacle avoidance system
 * - RF 433MHz remote control
 * 
 * Hardware Connections:
 * - L298N Motor Driver or L293D for main motor
 * - Servo motor for rudder control
 * - 4x HC-SR04 Ultrasonic Sensors
 * - RF 433MHz Receiver Module
 */

#include <Servo.h>

// Motor Driver Pins (L298N)
#define MOTOR_FWD 5    // Motor forward (PWM)
#define MOTOR_BWD 6    // Motor backward (PWM)

// Servo Pin
#define SERVO_PIN 9

// Ultrasonic Sensor Pins
// Forward Sensor
#define TRIG_FWD 2
#define ECHO_FWD 3

// Right Sensor
#define TRIG_RGT 4
#define ECHO_RGT 7

// Left Sensor
#define TRIG_LFT 8
#define ECHO_LFT 11

// Backward Sensor
#define TRIG_BWD 12
#define ECHO_BWD 13

// RF Receiver Pin
#define RF_RECEIVER A0

// Constants
#define SAFE_DISTANCE 30      // Minimum safe distance in cm
#define CRITICAL_DISTANCE 15  // Critical distance for emergency stop
#define MOTOR_SPEED 200       // Default motor speed (0-255)
#define MOTOR_SLOW 120        // Slow speed for turning

// Servo positions
#define SERVO_CENTER 90       // Straight
#define SERVO_LEFT 45         // Full left turn
#define SERVO_RIGHT 135       // Full right turn
#define SERVO_SLIGHT_LEFT 70  // Slight left
#define SERVO_SLIGHT_RIGHT 110 // Slight right

// Servo object
Servo rudderServo;

// Variables
int distanceFwd = 0;
int distanceRgt = 0;
int distanceLft = 0;
int distanceBwd = 0;

bool obstacleAvoidanceEnabled = true;
int currentSpeed = MOTOR_SPEED;
int currentServoPos = SERVO_CENTER;

// Command codes (adjust based on your remote)
enum Command {
  CMD_STOP = 0,
  CMD_FORWARD = 1,
  CMD_BACKWARD = 2,
  CMD_LEFT = 3,
  CMD_RIGHT = 4,
  CMD_FORWARD_LEFT = 5,
  CMD_FORWARD_RIGHT = 6,
  CMD_BACKWARD_LEFT = 7,
  CMD_BACKWARD_RIGHT = 8,
  CMD_TOGGLE_AVOIDANCE = 9
};

unsigned long lastCommandTime = 0;
#define COMMAND_TIMEOUT 500  // Stop if no command for 500ms

void setup() {
  Serial.begin(9600);
  
  // Initialize motor pins
  pinMode(MOTOR_FWD, OUTPUT);
  pinMode(MOTOR_BWD, OUTPUT);
  
  // Initialize servo
  rudderServo.attach(SERVO_PIN);
  rudderServo.write(SERVO_CENTER);
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_FWD, OUTPUT);
  pinMode(ECHO_FWD, INPUT);
  pinMode(TRIG_RGT, OUTPUT);
  pinMode(ECHO_RGT, INPUT);
  pinMode(TRIG_LFT, OUTPUT);
  pinMode(ECHO_LFT, INPUT);
  pinMode(TRIG_BWD, OUTPUT);
  pinMode(ECHO_BWD, INPUT);
  
  // Initialize RF receiver
  pinMode(RF_RECEIVER, INPUT);
  
  // Stop motor initially
  stopMotor();
  
  Serial.println("RC Boat (1 Motor) Initialized");
  Serial.println("Obstacle Avoidance: ENABLED");
}

void loop() {
  // Read all ultrasonic sensors
  distanceFwd = getDistance(TRIG_FWD, ECHO_FWD);
  distanceRgt = getDistance(TRIG_RGT, ECHO_RGT);
  distanceLft = getDistance(TRIG_LFT, ECHO_LFT);
  distanceBwd = getDistance(TRIG_BWD, ECHO_BWD);
  
  // Debug: Print distances every 1 second
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    printDistances();
    lastPrint = millis();
  }
  
  // Read remote control command
  int command = readRemoteCommand();
  
  // Execute command with obstacle avoidance
  if (command != CMD_STOP) {
    executeCommand(command);
    lastCommandTime = millis();
  }
  
  // Safety timeout - stop if no command received
  if (millis() - lastCommandTime > COMMAND_TIMEOUT) {
    stopMotor();
    centerRudder();
  }
  
  delay(50); // Small delay for stability
}

// Function to measure distance using ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms
  int distance = duration * 0.034 / 2;
  
  // Return 999 if no echo (no obstacle)
  if (distance == 0) return 999;
  
  return distance;
}

// Function to read remote control commands
int readRemoteCommand() {
  // Example using simple analog reading
  // Replace with your RF receiver library (RCSwitch, VirtualWire, etc.)
  
  int rfValue = analogRead(RF_RECEIVER);
  
  // Map analog values to commands (adjust thresholds for your remote)
  if (rfValue > 900) return CMD_FORWARD;
  else if (rfValue > 800) return CMD_BACKWARD;
  else if (rfValue > 700) return CMD_LEFT;
  else if (rfValue > 600) return CMD_RIGHT;
  else if (rfValue > 500) return CMD_FORWARD_LEFT;
  else if (rfValue > 400) return CMD_FORWARD_RIGHT;
  else if (rfValue > 300) return CMD_BACKWARD_LEFT;
  else if (rfValue > 200) return CMD_BACKWARD_RIGHT;
  else if (rfValue > 100) return CMD_TOGGLE_AVOIDANCE;
  else return CMD_STOP;
}

// Function to execute movement commands with safety checks
void executeCommand(int command) {
  switch (command) {
    case CMD_FORWARD:
      if (checkSafe(distanceFwd, "Forward")) {
        centerRudder();
        moveForward(currentSpeed);
        Serial.println("Moving Forward");
      } else {
        stopMotor();
        centerRudder();
        Serial.println("Obstacle ahead! Stopping.");
      }
      break;
      
    case CMD_BACKWARD:
      if (checkSafe(distanceBwd, "Backward")) {
        centerRudder();
        moveBackward(currentSpeed);
        Serial.println("Moving Backward");
      } else {
        stopMotor();
        centerRudder();
        Serial.println("Obstacle behind! Stopping.");
      }
      break;
      
    case CMD_LEFT:
      // Turn left in place (slow forward with full left rudder)
      turnLeft();
      Serial.println("Turning Left");
      break;
      
    case CMD_RIGHT:
      // Turn right in place (slow forward with full right rudder)
      turnRight();
      Serial.println("Turning Right");
      break;
      
    case CMD_FORWARD_LEFT:
      if (checkSafe(distanceFwd, "Forward") && checkSafe(distanceLft, "Left")) {
        setRudder(SERVO_SLIGHT_LEFT);
        moveForward(currentSpeed);
        Serial.println("Forward Left");
      } else {
        stopMotor();
        centerRudder();
      }
      break;
      
    case CMD_FORWARD_RIGHT:
      if (checkSafe(distanceFwd, "Forward") && checkSafe(distanceRgt, "Right")) {
        setRudder(SERVO_SLIGHT_RIGHT);
        moveForward(currentSpeed);
        Serial.println("Forward Right");
      } else {
        stopMotor();
        centerRudder();
      }
      break;
      
    case CMD_BACKWARD_LEFT:
      if (checkSafe(distanceBwd, "Backward")) {
        setRudder(SERVO_SLIGHT_RIGHT);  // Reverse steering
        moveBackward(currentSpeed);
        Serial.println("Backward Left");
      } else {
        stopMotor();
        centerRudder();
      }
      break;
      
    case CMD_BACKWARD_RIGHT:
      if (checkSafe(distanceBwd, "Backward")) {
        setRudder(SERVO_SLIGHT_LEFT);  // Reverse steering
        moveBackward(currentSpeed);
        Serial.println("Backward Right");
      } else {
        stopMotor();
        centerRudder();
      }
      break;
      
    case CMD_TOGGLE_AVOIDANCE:
      obstacleAvoidanceEnabled = !obstacleAvoidanceEnabled;
      Serial.print("Obstacle Avoidance: ");
      Serial.println(obstacleAvoidanceEnabled ? "ENABLED" : "DISABLED");
      break;
      
    case CMD_STOP:
    default:
      stopMotor();
      centerRudder();
      break;
  }
}

// Safety check function
bool checkSafe(int distance, String direction) {
  if (!obstacleAvoidanceEnabled) return true;
  
  if (distance < CRITICAL_DISTANCE) {
    Serial.print("CRITICAL! Obstacle at ");
    Serial.print(distance);
    Serial.print("cm in ");
    Serial.println(direction);
    return false;
  }
  
  if (distance < SAFE_DISTANCE) {
    Serial.print("Warning: Obstacle at ");
    Serial.print(distance);
    Serial.print("cm in ");
    Serial.println(direction);
    return false;
  }
  
  return true;
}

// Motor control functions
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

// Rudder/Steering control functions
void setRudder(int angle) {
  currentServoPos = constrain(angle, SERVO_LEFT, SERVO_RIGHT);
  rudderServo.write(currentServoPos);
}

void centerRudder() {
  setRudder(SERVO_CENTER);
}

void turnLeft() {
  setRudder(SERVO_LEFT);
  moveForward(MOTOR_SLOW);
}

void turnRight() {
  setRudder(SERVO_RIGHT);
  moveForward(MOTOR_SLOW);
}

// Debug function to print all distances
void printDistances() {
  Serial.print("FWD: ");
  Serial.print(distanceFwd);
  Serial.print("cm | RGT: ");
  Serial.print(distanceRgt);
  Serial.print("cm | LFT: ");
  Serial.print(distanceLft);
  Serial.print("cm | BWD: ");
  Serial.print(distanceBwd);
  Serial.print("cm | Rudder: ");
  Serial.println(currentServoPos);
}
