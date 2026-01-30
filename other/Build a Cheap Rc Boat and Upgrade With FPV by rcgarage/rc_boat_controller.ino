/*
 * RC Boat Controller (Transmitter)
 * 
 * This code runs on the controller Arduino with:
 * - Arduino Nano/Uno
 * - NRF24L01 radio module
 * - 2x Joysticks (throttle and steering)
 * - Push buttons for lights and FPV control
 * - OLED display (optional)
 * 
 * Author: Claude
 * Date: 2026
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Pin definitions
#define CE_PIN 9
#define CSN_PIN 10
#define JOYSTICK_THROTTLE A0
#define JOYSTICK_STEERING A1
#define BUTTON_LIGHTS 2
#define BUTTON_FPV 3
#define LED_TX_PIN 4

// Radio setup
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// Control structure (must match receiver)
struct ControlData {
  int throttle;
  int steering;
  bool lights;
  bool fpvRecord;
};

ControlData controlData;

// Button states
bool lightsState = false;
bool fpvState = false;
unsigned long lastLightsPress = 0;
unsigned long lastFpvPress = 0;
const unsigned long DEBOUNCE_DELAY = 200;

// Joystick calibration
int throttleCenter = 512;
int steeringCenter = 512;
const int DEADZONE = 50;

void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(BUTTON_LIGHTS, INPUT_PULLUP);
  pinMode(BUTTON_FPV, INPUT_PULLUP);
  pinMode(LED_TX_PIN, OUTPUT);
  
  // Calibrate joysticks (read center positions)
  delay(500);
  throttleCenter = analogRead(JOYSTICK_THROTTLE);
  steeringCenter = analogRead(JOYSTICK_STEERING);
  
  Serial.print("Throttle center: ");
  Serial.println(throttleCenter);
  Serial.print("Steering center: ");
  Serial.println(steeringCenter);
  
  // Initialize radio
  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {
      digitalWrite(LED_TX_PIN, !digitalRead(LED_TX_PIN));
      delay(100);
    }
  }
  
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  
  Serial.println("RC Boat Controller Initialized");
  
  // Startup indication
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_TX_PIN, HIGH);
    delay(200);
    digitalWrite(LED_TX_PIN, LOW);
    delay(200);
  }
}

void loop() {
  // Read joysticks
  int throttleRaw = analogRead(JOYSTICK_THROTTLE);
  int steeringRaw = analogRead(JOYSTICK_STEERING);
  
  // Apply deadzone and map to control range
  controlData.throttle = mapWithDeadzone(throttleRaw, throttleCenter, DEADZONE, -255, 255);
  controlData.steering = mapWithDeadzone(steeringRaw, steeringCenter, DEADZONE, -255, 255);
  
  // Read buttons with debouncing
  if (digitalRead(BUTTON_LIGHTS) == LOW && millis() - lastLightsPress > DEBOUNCE_DELAY) {
    lightsState = !lightsState;
    lastLightsPress = millis();
    Serial.print("Lights: ");
    Serial.println(lightsState ? "ON" : "OFF");
  }
  
  if (digitalRead(BUTTON_FPV) == LOW && millis() - lastFpvPress > DEBOUNCE_DELAY) {
    fpvState = !fpvState;
    lastFpvPress = millis();
    Serial.print("FPV Record: ");
    Serial.println(fpvState ? "ON" : "OFF");
  }
  
  controlData.lights = lightsState;
  controlData.fpvRecord = fpvState;
  
  // Transmit data
  bool success = radio.write(&controlData, sizeof(ControlData));
  
  // Indicate transmission status
  digitalWrite(LED_TX_PIN, success ? HIGH : LOW);
  
  // Debug output
  if (millis() % 500 < 20) {
    Serial.print("TX: T=");
    Serial.print(controlData.throttle);
    Serial.print(" S=");
    Serial.print(controlData.steering);
    Serial.print(" | Status: ");
    Serial.println(success ? "OK" : "FAIL");
  }
  
  delay(20); // 50Hz update rate
}

int mapWithDeadzone(int value, int center, int deadzone, int minOut, int maxOut) {
  // Apply deadzone
  if (abs(value - center) < deadzone) {
    return 0;
  }
  
  // Map to output range
  if (value > center + deadzone) {
    return map(value, center + deadzone, 1023, 0, maxOut);
  } else {
    return map(value, 0, center - deadzone, minOut, 0);
  }
}
