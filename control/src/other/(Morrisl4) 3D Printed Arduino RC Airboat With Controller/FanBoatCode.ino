//Fan Boat Code

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
char receivedData[32] = "";
int xAxis, yAxis;

Servo myServo;
int In3 = 7;
int In4 = 6;
int ENB = 5;
int delaytime=20;
 
void setup() {
  pinMode(In3,OUTPUT);
  pinMode(In4,OUTPUT);
  pinMode(ENB,OUTPUT);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  Serial.begin(9600);
  myServo.attach(3);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData));
    xAxis=atoi(&receivedData[0]);
    delay(delaytime);
    radio.read(&receivedData, sizeof(receivedData));
    yAxis=atoi(&receivedData[0]);
    delay(delaytime);
    
  }
  int angelV= map(xAxis, 0, 1023, 130, 60);
  int throttle= map(yAxis, 0, 1023, 0, 255);
  myServo.write(angelV);
  analogWrite(ENB, throttle); 
  
  Serial.print(angelV);
  Serial.print("\t"); 
  Serial.print(yAxis);
  Serial.print("\t");
  Serial.print(throttle);
  Serial.println();
   
}
