#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#define LED 2

const int potpin = 25;
Servo servotest;
int pos = 0;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    Serial.println("ESP32 measuring Test:");
    servotest.attach(26);
}

void loop()
{
    digitalWrite(LED, HIGH);
    Serial.println("LED is on");
    delay(1000);
    digitalWrite(LED, LOW);
    Serial.println("LED is off");
    delay(1000);

    int sensorValue = analogRead(potpin);
    float voltage = sensorValue * (5.0 / 4096.0);
    Serial.println(voltage);

    for (pos = 0; pos <= 180; pos += 1)
    {
        servotest.write(pos);
        delay(150);
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {
        servotest.write(pos);
        delay(150);
    }
    servotest.write(0);
    delay(2000);
}
