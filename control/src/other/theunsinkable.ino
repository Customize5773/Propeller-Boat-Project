#include <Servo.h>

Servo myservo1;
Servo myservo2;

int potpin1 = 0;
int potpin2 = 1;

int val;
int val2;

void setup() {
    myservo1.attach(8);
    myservo2.attach(9);
}

void loop() {
    val = analogRead(potpin1);
    val = map(val, 0, 1023, 0, 180);
    myservo1.write(val);

    val2 = analogRead(potpin2);
    val2 = map(val2, 0, 1023, 0, 180);
    myservo2.write(val2);
}