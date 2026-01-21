 //Transmitter Joystick

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9); //CE, CSN
const byte address[6] = "00001";

char xyData[32] = "";
String Rjoy, Lpot, Rpot, Ljoy;

int delaytime = 20;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  Rjoy = analogRead(A1);//Read Right joystick values from VRy
  Lpot = analogRead(A2);//Read Lpot values
  Rpot = analogRead(A3);
  Ljoy = analogRead(A0);

  Serial.print("Ljoy = ");
  Serial.print(Ljoy);
  Serial.print("\t");
  
  Serial.print("Rjoy = ");
  Serial.print(Rjoy);
  Serial.print("\t");
  
  Serial.print("Lpot = ");
  Serial.print(Lpot);
  Serial.print("\t");
  
  Serial.print("Rpot = ");
  Serial.print(Rpot);
  
  Serial.println();
  
  //X value
  Rjoy.toCharArray(xyData, 5);//Put the String (X Value) into a character array
  radio.write(&xyData, sizeof(xyData));//Send the array data (X-value) to the other NRF24L01 modile
  
  //Y value
  Lpot.toCharArray(xyData, 5);
  radio.write(&xyData, sizeof(xyData));
  delay(delaytime);
  
}
