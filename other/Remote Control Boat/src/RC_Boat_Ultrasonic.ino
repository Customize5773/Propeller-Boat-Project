/*
 * Remote Control Boat with 2 Motors and 4 Ultrasonic Sensors
 * 
 * Features:
 * - 2 DC motors for left and right propulsion
 * - 4 ultrasonic sensors (Forward, Right, Left, Backward)
 * - Obstacle avoidance system
 * - RF 433MHz remote control
 * 
 * Hardware Connections:
 * - L298N Motor Driver or L293D Motor Shield
 * - 4x HC-SR04 Ultrasonic Sensors
 * - RF 433MHz Receiver Module
 */

// Motor Driver Pins (L298N)
#define MOTOR_LEFT_FWD 5    // Left motor forward (PWM)
#define MOTOR_LEFT_BWD 6    // Left motor backward (PWM)
#define MOTOR_RIGHT_FWD 9   // Right motor forward (PWM)
#define MOTOR_RIGHT_BWD 10  // Right motor backward (PWM)

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
#define TURN_SPEED 150        // Speed for turning

// Variables
int distanceFwd = 0;
int distanceRgt = 0;
int distanceLft = 0;
int distanceBwd = 0;

bool obstacleAvoidanceEnabled = true;
int currentSpeed = MOTOR_SPEED;

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

void setup() {
  Serial.begin(9600);
  
  // Initialize motor pins
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);
  
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
  
  // Stop motors initially
  stopMotors();
  
  Serial.println("RC Boat Initialized");
  Serial.println("Obstacle Avoidance: ENABLED");
}

void loop() {
  // Read all ultrasonic sensors
  distanceFwd = getDistance(TRIG_FWD, ECHO_FWD);
  distanceRgt = getDistance(TRIG_RGT, ECHO_RGT);
  distanceLft = getDistance(TRIG_LFT, ECHO_LFT);
  distanceBwd = getDistance(TRIG_BWD, ECHO_BWD);
  
  // Debug: Print distances
  printDistances();
  
  // Read remote control command
  int command = readRemoteCommand();
  
  // Execute command with obstacle avoidance
  executeCommand(command);
  
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
// NOTE: Implement based on your specific RF module
// This is a placeholder - replace with your RF library code
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
  else return CMD_STOP;
  
  /* 
   * Alternative using RCSwitch library:
   * 
   * #include <RCSwitch.h>
   * RCSwitch mySwitch = RCSwitch();
   * 
   * In setup():
   * mySwitch.enableReceive(digitalPinToInterrupt(RF_RECEIVER));
   * 
   * In this function:
   * if (mySwitch.available()) {
   *   long value = mySwitch.getReceivedValue();
   *   mySwitch.resetAvailable();
   *   
   *   switch(value) {
   *     case 1234: return CMD_FORWARD;
   *     case 5678: return CMD_BACKWARD;
   *     // ... etc
   *   }
   * }
   */
}

// Function to execute movement commands with safety checks
void executeCommand(int command) {
  switch (command) {
    case CMD_FORWARD:
      if (checkSafe(distanceFwd, "Forward")) {
        moveForward(currentSpeed);
      } else {
        stopMotors();
        Serial.println("Obstacle ahead! Stopping.");
      }
      break;
      
    case CMD_BACKWARD:
      if (checkSafe(distanceBwd, "Backward")) {
        moveBackward(currentSpeed);
      } else {
        stopMotors();
        Serial.println("Obstacle behind! Stopping.");
      }
      break;
      
    case CMD_LEFT:
      turnLeft(TURN_SPEED);
      break;
      
    case CMD_RIGHT:
      turnRight(TURN_SPEED);
      break;
      
    case CMD_FORWARD_LEFT:
      if (checkSafe(distanceFwd, "Forward") && checkSafe(distanceLft, "Left")) {
        moveForwardLeft(currentSpeed);
      } else {
        stopMotors();
      }
      break;
      
    case CMD_FORWARD_RIGHT:
      if (checkSafe(distanceFwd, "Forward") && checkSafe(distanceRgt, "Right")) {
        moveForwardRight(currentSpeed);
      } else {
        stopMotors();
      }
      break;
      
    case CMD_BACKWARD_LEFT:
      if (checkSafe(distanceBwd, "Backward")) {
        moveBackwardLeft(currentSpeed);
      } else {
        stopMotors();
      }
      break;
      
    case CMD_BACKWARD_RIGHT:
      if (checkSafe(distanceBwd, "Backward")) {
        moveBackwardRight(currentSpeed);
      } else {
        stopMotors();
      }
      break;
      
    case CMD_TOGGLE_AVOIDANCE:
      obstacleAvoidanceEnabled = !obstacleAvoidanceEnabled;
      Serial.print("Obstacle Avoidance: ");
      Serial.println(obstacleAvoidanceEnabled ? "ENABLED" : "DISABLED");
      break;
      
    case CMD_STOP:
    default:
      stopMotors();
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
  analogWrite(MOTOR_LEFT_FWD, speed);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, speed);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}

void moveBackward(int speed) {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, speed);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, speed);
}

void turnLeft(int speed) {
  // Left motor backward, right motor forward
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, speed);
  analogWrite(MOTOR_RIGHT_FWD, speed);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}

void turnRight(int speed) {
  // Left motor forward, right motor backward
  analogWrite(MOTOR_LEFT_FWD, speed);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, speed);
}

void moveForwardLeft(int speed) {
  // Reduce left motor speed for gradual turn
  analogWrite(MOTOR_LEFT_FWD, speed * 0.5);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, speed);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}

void moveForwardRight(int speed) {
  // Reduce right motor speed for gradual turn
  analogWrite(MOTOR_LEFT_FWD, speed);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, speed * 0.5);
  analogWrite(MOTOR_RIGHT_BWD, 0);
}

void moveBackwardLeft(int speed) {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, speed * 0.5);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, speed);
}

void moveBackwardRight(int speed) {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, speed);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, speed * 0.5);
}

void stopMotors() {
  analogWrite(MOTOR_LEFT_FWD, 0);
  analogWrite(MOTOR_LEFT_BWD, 0);
  analogWrite(MOTOR_RIGHT_FWD, 0);
  analogWrite(MOTOR_RIGHT_BWD, 0);
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
  Serial.println("cm");
}
