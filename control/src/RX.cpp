
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

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe



RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

Servo ESC;
Servo Turn;



int joystick[2];  // 2 element array holding Joystick readings
int Throttle = 50;
int Steering = 90;
int fail = 0;

void setup()   
{
  Serial.begin(250000);
  delay(100);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  ESC.attach(2);
  Turn.attach(4);
}//--(end setup )-


void loop()   
{
  if ( radio.available() )
  {
    {
      // Fetch the data payload
      radio.read( joystick, sizeof(joystick) );

      Throttle = map(joystick[0], 0, 1023, 1000, 2000);
      Steering = map(joystick[1], 50, 720, 50, 120);

      Serial.print("Throttle = ");
      Serial.print(joystick[0]);
      Serial.print(" Steering = ");
      Serial.print(joystick[1]);
      Serial.print(" Throttle Adjusted = ");
      Serial.print(Throttle);
      Serial.print(" Steering Adjusted = ");
      Serial.println(Steering);

      fail = 0;
    }
  }
  else
  {
    Serial.println("No radio available");
    fail++;
    if (fail > 200){
      Throttle = 0;
      Steering = 90;
    }
  }
  
  ESC.write(Throttle);
  Turn.write(Steering);
}
