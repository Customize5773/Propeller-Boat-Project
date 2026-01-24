/*
 * This little sketch makes an arduino act like an ON Semiconductor NCP5623 I2C RGB LED controller. 
 * Here is a link to the datasheet: https://www.onsemi.com/pub/Collateral/NCP5623-D.PDF
 * It takes the I2C commands and translates them to a NeoPixel LED strip.
 * 
 * It is used in combination with an ArduPilot controller. 
 * In the ArduPilot Firmware, the Parameter NTF_LED_TYPE has to be set to 128 aka. NCP5623 Internal.
 * Before the Ardupilot is bootet and sends I2C Signals, the leds will do a breathing animation, while cycling through the colors.
 *
 * PArts of the code are taken from the Adafruit NeoPixel example.
 */

#include <Adafruit_NeoPixel.h>
#define PIN        6
#define NUMPIXELS 36
#define STRIP_LENGTH 18

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
bool setupDone = false;
bool initialized = false;

#include <Wire.h>
enum Channel {RED_PWM = 2,GREEN_PWM = 3, BLUE_PWM = 4};

//Function Prototypes
void processInput(uint8_t receivedByte);
void printColors();
uint8_t translateTo8BitRange(uint8_t fiveBit);
void updateLeds();
uint32_t colorWheel(byte WheelPos,uint8_t brightness);


void setup() {
  Wire.begin(0x38);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  pixels.begin();
  pixels.clear();
  pixels.show();
  Serial.begin(9600);           // start serial for output
  Serial.println("Ready");
  uint8_t wheelPos = 0;
  while(!initialized){
    initAnimation(wheelPos);
    wheelPos += 30;
  }
  setupDone=true;
}

void loop() {
  delay(100);
  //empty loop
}

void receiveEvent(int numberOfBytes) {
  initialized = true;
  while (Wire.available()) { // loop through all but the last
    uint8_t receivedByte = Wire.read();
    processInput(receivedByte);
   }
}

enum Direction {REVERSED = -1, FORWARD = 1};

void writeToStrip(int startPixel, int endPixel, int direction, int value, int offset, uint8_t wheelPos){
  int partLength = endPixel-startPixel;
  
  for(int i = startPixel; i < endPixel; i++){
    int brightness = 0;
    if(direction == FORWARD){
      brightness = (value - offset*(i-startPixel));
    }
    else if(direction == REVERSED){
      brightness = (value - offset*(partLength-(i-startPixel)));
    }
    
    if(brightness < 0){
      brightness = 0;
      }
    else if(brightness > 255){
      brightness = 255;
    }
    pixels.setPixelColor(i, colorWheel(wheelPos,brightness));
  }
}

void initAnimation(uint8_t wheelPos){
const int brightnessOffset = 20;
for(int brightnessCounter = 0; brightnessCounter < ( 256 + ((STRIP_LENGTH/2 - 1)*brightnessOffset ) );brightnessCounter+=10){ 
  Serial.println(brightnessCounter); 
  writeToStrip(0,STRIP_LENGTH/2,REVERSED,brightnessCounter,brightnessOffset,wheelPos);
  writeToStrip(STRIP_LENGTH/2,STRIP_LENGTH,FORWARD,brightnessCounter,brightnessOffset,wheelPos);
  writeToStrip(STRIP_LENGTH,STRIP_LENGTH*1.5,REVERSED,brightnessCounter,brightnessOffset,wheelPos);
  writeToStrip(STRIP_LENGTH*1.5,STRIP_LENGTH*2,FORWARD,brightnessCounter,brightnessOffset,wheelPos);
  pixels.show();
  delay(50);
}
for(int brightnessCounter = ( 256 + ((STRIP_LENGTH/2 - 1)*brightnessOffset ) ); brightnessCounter > 0;brightnessCounter-=10){  
  writeToStrip(0,STRIP_LENGTH/2,REVERSED,brightnessCounter,brightnessOffset,wheelPos);
  writeToStrip(STRIP_LENGTH/2,STRIP_LENGTH,FORWARD,brightnessCounter,brightnessOffset,wheelPos);
  writeToStrip(STRIP_LENGTH,STRIP_LENGTH*1.5,REVERSED,brightnessCounter,brightnessOffset,wheelPos);
  writeToStrip(STRIP_LENGTH*1.5,STRIP_LENGTH*2,FORWARD,brightnessCounter,brightnessOffset,wheelPos);
  pixels.show();
  delay(50);
}
}

void processInput(uint8_t receivedByte){
  Channel channel = (receivedByte & B11100000) >> 5;
  uint8_t colorValue = (receivedByte & B00011111);

  switch(channel){
    case RED_PWM: red = translateTo8BitRange(colorValue);
            break;
    case GREEN_PWM: green = translateTo8BitRange(colorValue);
            break;
    case BLUE_PWM: blue = translateTo8BitRange(colorValue);
            break;
    default: break;
  }
  if(setupDone){
  updateLeds();
  }
}

void printColors(){
  Serial.print("R "); Serial.print(red); 
  Serial.print(" | G "); Serial.print(green);
  Serial.print(" | B "); Serial.print(blue);
  Serial.println();
}
  
uint8_t translateTo8BitRange(uint8_t fiveBit){
  if(fiveBit > 0){
  return (31/fiveBit) * 255;
  }
  else{
    return 0;
  }
}

void updateLeds(){
  for(int i = 0; i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
  }
  pixels.show();
}

uint32_t colorWheel(byte WheelPos,uint8_t brightness = 255) {
  float brightnessFactor = 0.00;
  if(brightness > 0){
    brightnessFactor = float(brightness)/255;
  }
  
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color( brightnessFactor*(255 - WheelPos * 3), 0, brightnessFactor*(WheelPos * 3) );
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, brightnessFactor*(WheelPos * 3) , brightnessFactor*(255 - WheelPos * 3));
  }
  WheelPos -= 170;
  return pixels.Color( brightnessFactor*(WheelPos * 3), brightnessFactor*(255 - WheelPos * 3), 0);
}
