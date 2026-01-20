/* YourDuinoStarter Example: nRF24L01 Transmit Joystick values
 - WHAT IT DOES: Reads Analog values on A0, A1 and transmits
   them over a nRF24L01 Radio Link to another transceiver.
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
   - 
   Analog Joystick or two 10K potentiometers:
   GND to Arduino GND
   VCC to Arduino +5V
   X Pot to Arduino A0
   Y Pot to Arduino A1
   
 entirely Shaz's coding
 e-mail: shaznifazal@yahoo.com*/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10
#define JOYSTICK_X A0
#define JOYSTICK_Y A1

const uint64_t pipe = 0xE8E8F0F0E1LL;
const int BAUD_RATE = 250000;

RF24 radio(CE_PIN, CSN_PIN);
int joystick[2];

void setup() {
  Serial.begin(BAUD_RATE);
  delay(100);
  
  Serial.println("\n=== nRF24L01 Transmitter Initialized ===");
  
  if (!radio.begin()) {
    Serial.println("ERROR: Radio initialization failed!");
    while (1);  // Halt if radio fails
  }
  
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipe);
  radio.stopListening();  // Transmit mode
  
  Serial.println("Transmitter ready - sending data...");
}

void loop() {
  joystick[0] = analogRead(JOYSTICK_X);
  joystick[1] = analogRead(JOYSTICK_Y);
  
  if (radio.write(joystick, sizeof(joystick))) {
    Serial.print("TX - X: ");
    Serial.print(joystick[0]);
    Serial.print(" | Y: ");
    Serial.println(joystick[1]);
  } else {
    Serial.println("WARNING: Transmission failed");
  }
  
  delay(50);  // Send data every 50ms (~20 Hz)
}





