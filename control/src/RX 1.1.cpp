/* YourDuinoStarter Example: nRF24L01 Receive Joystick values

  - WHAT IT DOES: Receives data from another transceiver with
   2 Analog values from a Joystick or 2 Potentiometers
   Displays received values on Serial Monitor
  - SEE the comments after "//" on each line below
  - CONNECTIONS: nRF24L01 Modules See:
  http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED

 entirely Shazni's coding
 e-mail: shaznifazal@yahoo.com */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE_PIN   9
#define CSN_PIN 10
#define THROTTLE_PIN 2
#define STEERING_PIN 4

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);
Servo ESC;
Servo Turn;

int joystick[2];
int throttleValue = 1500;  // Neutral throttle (ESC range: 1000-2000)
int steeringValue = 90;    // Center steering (range: 50-120)
int failCounter = 0;
const int FAIL_THRESHOLD = 200;
const int BAUD_RATE = 250000;

void setup() {
  Serial.begin(BAUD_RATE);
  delay(100);
  
  Serial.println("\n=== nRF24L01 Receiver Initialized ===");
  
  // Initialize radio
  if (!radio.begin()) {
    Serial.println("ERROR: Radio initialization failed!");
  }
  
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  
  // Initialize servos
  ESC.attach(THROTTLE_PIN);
  Turn.attach(STEERING_PIN);
  
  // Set safe initial values
  ESC.writeMicroseconds(1000);
  Turn.write(90);
  
  Serial.println("All systems ready.");
}

void loop() {
  if (radio.available()) {
    radio.read(joystick, sizeof(joystick));
    
    // Map joystick values to servo ranges
    throttleValue = map(joystick[0], 0, 1023, 1000, 2000);
    steeringValue = map(joystick[1], 50, 720, 50, 120);
    
    // Debug output
    Serial.print("RX - Throttle: ");
    Serial.print(joystick[0]);
    Serial.print(" (");
    Serial.print(throttleValue);
    Serial.print("µs) | Steering: ");
    Serial.print(joystick[1]);
    Serial.print(" (");
    Serial.print(steeringValue);
    Serial.println("°)");
    
    failCounter = 0;
  }
  else {
    failCounter++;
    
    if (failCounter == 1) {
      Serial.println("WARNING: No signal received");
    }
    
    if (failCounter > FAIL_THRESHOLD) {
      throttleValue = 1000;  // Cut throttle
      steeringValue = 90;    // Center steering
      Serial.println("FAILSAFE: Signal lost - returning to neutral");
      failCounter = FAIL_THRESHOLD;  // Prevent overflow
    }
  }
  
  // Write values to servos
  ESC.writeMicroseconds(throttleValue);
  Turn.write(steeringValue);
  
  delay(10);  // Small delay to prevent overwhelming the servos
}
