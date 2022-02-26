#include <ESP32Servo.h>
#include "Arduino.h"

Servo a1_servo;
Servo a2_servo;
Servo a3_servo;
Servo a4_servo;
Servo a5_servo;

int a1_servoPin = 2;
int a2_servoPin = 4;
int a3_servoPin = 15;
int a4_servoPin = 16;
int a5_servoPin = 17;

int pos = 0;

void setup()
{
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    a1_servo.setPeriodHertz(50); // standard 50 hz servo
    a1_servo.attach(a1_servoPin, 100, 2400);
    a2_servo.setPeriodHertz(50);
    a2_servo.attach(a2_servoPin, 100, 2400);
    a3_servo.setPeriodHertz(50);
    a3_servo.attach(a3_servoPin, 100, 2400);
    a4_servo.setPeriodHertz(50);
    a4_servo.attach(a4_servoPin, 100, 2400);
    a5_servo.setPeriodHertz(50);
    a5_servo.attach(a5_servoPin, 100, 2400);
}

void loop()
{
    for (pos = 0; pos <= 180; pos += 1)
    {
        a1_servo.write(pos);
        delay(15);
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {
        a1_servo.write(pos);
        delay(15);
    }
}