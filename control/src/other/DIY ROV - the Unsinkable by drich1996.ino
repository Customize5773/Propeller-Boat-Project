#include <Wire.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Sensor.h>
#include <Button.h>

const int potpin1 = 1;
const int potpin = 0;
int pan = 0;
int tilt = 1;

// Init Display (SCLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
// Init HMC5883L sensor
Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(12345);

// Constants
const int lcdLight = 13;
Button buttonA = Button(2, PULLUP);
Button buttonB = Button(8, PULLUP);
static unsigned char Leters[] = { 'N', 'E', 'S', 'W' };
const static unsigned char PROGMEM arrow_bmp[] = {B00100000, B00100000, B01110000, B01110000, B11111000};

int r = 24;
int x0 = 60;
int y0 = 24;

// Variables
int flag = 0;
int i = 1;

void setup() {
    delay(10000);
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("Serial Monitor Connected");
    Serial.parseInt();

    pinMode(lcdLight, OUTPUT);
    compass.begin();
    Wire.begin();
    display.begin();
    display.setContrast(30);

    digitalWrite(lcdLight, HIGH);
    display.clearDisplay();
    display.setTextColor(BLACK);
    display.setCursor(0, 2);
    display.print(" ROV ");
    display.setCursor(0, 13);
    display.print(" Compass ");
    display.setCursor(0, 23);
    display.print(" ");
    display.setCursor(0, 33);
    display.print(" Derrico Industries");
    display.display();
    digitalWrite(lcdLight, HIGH);
    delay(10000);
    display.clearDisplay();
}

void loop() {
    pan = analogRead(potpin);
    tilt = analogRead(potpin1);
    Serial1.println(pan);
    Serial1.println(tilt);
    delay(10);

    if (Serial1.available()) {
        delay(10);
    }

    if (buttonB.isPressed()) {
        delay(100);
        flag = (flag == 0) ? 1 : 0;
    }

    if (buttonA.isPressed()) {
        i = (i == 0) ? 1 : 0;
    }

    digitalWrite(lcdLight, (i == 0) ? HIGH : LOW);

    sensors_event_t event;
    compass.getEvent(&event);
    float heading = atan2(event.magnetic.y, event.magnetic.x);
    float declinationAngle = 0.22;
    heading += declinationAngle;

    if (heading < 0)
        heading += 2 * PI;
    if (heading > 2 * PI)
        heading -= 2 * PI;

    float headingDegrees = heading * 180 / M_PI;
    int angle = (int)headingDegrees;

    display.clearDisplay();

    if (flag == 0) {
        DrawRow(angle);
        display.drawBitmap(40, 24, arrow_bmp, 5, 5, 1);
        display.drawLine(42, 0, 42, 24, BLACK);
        display.setCursor(32, 34);
        display.print(angle);
        display.setTextSize(1);
        display.print("o");
        display.display();
        delay(500);
    }

    if (flag == 1) {
        DrawCircle(angle);
        display.setTextSize(2);
        display.setTextColor(BLACK);
        display.setCursor(0, 0);
        display.println(angle);
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(x0 - 2, 0);
        display.println("N");
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor((x0 + r) - 5, y0 - 3);
        display.println("E");
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(x0 - 2, y0 + r - 8);
        display.println("S");
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor((x0 - r) + 5, y0 - 3);
        display.println("W");
        display.drawTriangle(0, 46, 20, 46, 10, 18, BLACK);
        display.fillTriangle(0, 46, 20, 46, 10, 18, BLACK);
        display.display();
        delay(150);
    }
}

void DrawRow(int angle) {
    display.drawLine(0, 0, 84, 0, BLACK);
    display.drawLine(0, 1, 84, 1, BLACK);
    display.drawLine(0, 22, 91, 22, BLACK);
    display.drawLine(0, 23, 95, 23, BLACK);
    display.setTextSize(2);
    display.setTextColor(BLACK);

    int start = 42 - angle / 3;
    if (start > 120)
        start += -120;

    int x = 0;
    int y = 18;

    for (int i = 0; i < 4; i++) {
        x = start + (i * 30) - 1;
        if (x > 119)
            x += -120;
        display.drawPixel(x + 1, y - 2, 1);
        display.drawPixel(x, y, 1);
        display.drawPixel(x + 1, y, 1);
        display.drawPixel(x + 2, y, 1);
        display.drawPixel(x, y - 1, 1);
        display.drawPixel(x + 1, y - 1, 1);
        display.drawPixel(x + 2, y - 1, 1);
        display.setCursor((x - 4), (y - 16));
        display.write(Leters[i]);
    }

    for (int i = 0; i < 24; i++) {
        x = start + (i * 5) - 1;
        if (x > 119)
            x += -120;
        display.drawPixel(x + 1, y + 1, 1);
        display.drawPixel(x, y + 2, 1);
        display.drawPixel(x + 1, y + 2, 1);
        display.drawPixel(x + 2, y + 2, 1);
        display.drawPixel(x, y + 3, 1);
        display.drawPixel(x + 1, y + 3, 1);
        display.drawPixel(x + 2, y + 3, 1);
    }

    for (int i = 0; i < 8; i++) {
        x = start + (i * 15) - 1;
        if (x > 119)
            x += -120;
        display.drawPixel(x + 1, y - 1, 1);
        display.drawPixel(x, y, 1);
        display.drawPixel(x + 1, y, 1);
        display.drawPixel(x + 2, y, 1);
        display.drawPixel(x, y + 1, 1);
        display.drawPixel(x + 1, y + 1, 1);
        display.drawPixel(x + 2, y + 1, 1);
    }
}

void DrawCircle(int angle) {
    int needleAngle = angle;

    if (needleAngle >= 0 && needleAngle <= 45) {
        needleAngle = needleAngle / 2;
        display.drawLine(x0, y0, x0 + needleAngle, y0 - r, BLACK);
    }
    else if (needleAngle > 45 && needleAngle <= 90) {
        needleAngle = (needleAngle - 44) / 2;
        display.drawLine(x0, y0, x0 + r, (y0 - r) + needleAngle, BLACK);
    }
    else if (needleAngle > 90 && needleAngle <= 135) {
        needleAngle = (needleAngle - 90) / 2;
        display.drawLine(x0, y0, x0 + r, y0 + needleAngle, BLACK);
    }
    else if (needleAngle > 135 && needleAngle <= 180) {
        needleAngle = (needleAngle - 134) / 2;
        display.drawLine(x0, y0, (x0 + r) - needleAngle, y0 + r, BLACK);
    }
    else if (needleAngle > 180 && needleAngle <= 225) {
        needleAngle = (needleAngle - 178) / 2;
        display.drawLine(x0, y0, x0 - needleAngle, y0 + r, BLACK);
    }
    else if (needleAngle > 225 && needleAngle <= 270) {
        needleAngle = (needleAngle - 222) / 2;
        display.drawLine(x0, y0, x0 - r, (y0 + r) - needleAngle, BLACK);
    }
    else if (needleAngle > 270 && needleAngle <= 315) {
        needleAngle = (needleAngle - 270) / 2;
        display.drawLine(x0, y0, x0 - r, y0 - needleAngle, BLACK);
    }
    else if (needleAngle > 315 && needleAngle <= 360) {
        needleAngle = (needleAngle - 312) / 2;
        display.drawLine(x0, y0, (x0 - r) + needleAngle, y0 - r, BLACK);
    }
}