// 第一组测试
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include "Arduino.h"

Servo a1_servo;
Servo a2_servo;
Servo a3_servo;
Servo a4_servo;
Servo a5_servo;

// int a1_pos = 0;
// int a2_pos = 0;
// int a3_pos = 0;
// int a4_pos = 0;
// int a5_pos = 0;

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33,记得改
int a1_servoPin = 26;
int a2_servoPin = 4;
int a3_servoPin = 15;
int a4_servoPin = 16;
int a5_servoPin = 17;

// receive data structure,Must match the sender structure
typedef struct data_structure
{
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
} data_structure;

// Create a struct_message called received_data
data_structure received_data;

// callback function that will be executed when ESP-NOW data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    memcpy(&received_data, incomingData, sizeof(received_data));
    Serial.println();
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("a1: ");
    Serial.println(received_data.a1);
    Serial.print("a2: ");
    Serial.println(received_data.a2);
    Serial.print("a3: ");
    Serial.println(received_data.a3);
    Serial.print("a4: ");
    Serial.println(received_data.a4);
    Serial.print("a5: ");
    Serial.println(received_data.a5);

    a1_servo.write(int(received_data.a1));
    a2_servo.write(int(received_data.a2));
    a3_servo.write(int(received_data.a3));
    a4_servo.write(int(received_data.a4));
    a5_servo.write(int(received_data.a5));
}

void setup()
{
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    //这里为什么是2400.你看那个介绍就知道，int min，int max，为什么啊
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

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
}
