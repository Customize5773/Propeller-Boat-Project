// Arduino Bluetooth Boat
#include <Servo.h>
Servo myServo;

int val;
int LED = 13;
int IN4 = 4;
int IN3 = 5;
int IN2 = 7;
int IN1 = 6;
int k = 0;
void setup()
{
    myServo.attach(10);
    pinMode(LED, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
    {
        val = Serial.read();
        if (k == 0)
        {
            myServo.write(90); // (1)
            k++;
        }
        // "Forward" (W)
        if (val == 'X')
        {
            myServo.write(90);
        }
        if (val == 'x')
        {
            myServo.write(90);
        }
        if (val == 'F')
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }
        // "Backward" (S)
        else if (val == 'S')
        {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }
        // "B" 
        if (val == 'B')
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }
        // "L"
        if (val == 'L')
        {
            myServo.write(50);
            // myservo.write(90);
        }
        if (val == 'R')
        {
            myServo.write(130);
            // myservo.write(90);
        }

        // 45 +

        // "G"
        if (val == 'G')
        {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            mysServo.write(50);
            delay(1000);
            myServo.write(90);
        }

        // "I"
        if (val == 'I')
        {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            myServo.write(130);
            delay(1000);
            myServo.write(90);
        }
        // "H"
        if (val == 'H')
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            myServo.write(130);
            delay(1000);
            myServo.write(90);
        }
        // "J"
        if (val == 'J')
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            myServo.write(50);
            delay(1000);
            myServo.write(90);
        }
    }
}