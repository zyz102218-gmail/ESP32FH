#include <WiFi.h>
#include <esp_now.h>
#include "Arduino.h"

uint8_t broadcastAddress1[] = {0x94, 0xB9, 0x7E, 0xDE, 0xE2, 0xF4};
// if you have other receivers you can define in the following
// uint8_t broadcastAddress2[] = {0xFF, , , , , };
// uint8_t broadcastAddress3[] = {0xFF, , , , , };
// 接收端的mac：94:B9:7E:E9:43:A8

typedef struct data_structure
{
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
} data_structure;

data_structure sending_data; //记得这里改引脚
const int hands_a1 = 32;     //拇指
const int hands_a2 = 35;     //食指
const int hands_a3 = 34;     //中指
const int hands_a4 = 39;     //无名指
const int hands_a5 = 36;     //小指

// 收到请回答
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    char macStr[18];
    Serial.print("Packet to: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
    Serial.println("send status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    pinMode(hands_a1, INPUT);
    pinMode(hands_a2, INPUT);
    pinMode(hands_a3, INPUT);
    pinMode(hands_a4, INPUT);
    pinMode(hands_a5, INPUT);

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);

    // register peer
    esp_now_peer_info_t peerInfo;
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    // register first peer
    memcpy(peerInfo.peer_addr, broadcastAddress1, 6);

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop()
{
    float a1_degree = analogRead(hands_a1);
    float a2_degree = analogRead(hands_a2);
    float a3_degree = analogRead(hands_a3);
    float a4_degree = analogRead(hands_a4);
    float a5_degree = analogRead(hands_a5);

    a1_degree = a1_degree * (5.0 / 4095.0);
    a2_degree = a2_degree * (5.0 / 4095.0);
    a3_degree = a3_degree * (5.0 / 4095.0);
    a4_degree = a4_degree * (5.0 / 4095.0);
    a5_degree = a5_degree * (5.0 / 4095.0);

    a1_degree = map(a1_degree, 0, 5, 40, 135);
    a2_degree = map(a2_degree, 0, 5, 90, 180);
    a3_degree = map(a3_degree, 0, 5, 30, 160);
    a4_degree = map(a4_degree, 0, 5, 40, 120);
    a5_degree = map(a5_degree, 0, 5, 40, 110);

    Serial.print("a1_degree: ");
    Serial.print(a1_degree, 0); // 四舍五入到第一个小数位
    Serial.print("\n");
    Serial.print("a2_degree: ");
    Serial.print(a2_degree, 0); // 四舍五入到第一个小数位
    Serial.print("\n");
    Serial.print("a3_degree: ");
    Serial.print(a3_degree, 0); // 四舍五入到第一个小数位
    Serial.print("\n");
    Serial.print("a4_degree: ");
    Serial.print(a4_degree, 0); // 四舍五入到第一个小数位
    Serial.print("\n");
    Serial.print("a5_degree: ");
    Serial.print(a5_degree, 0); // 四舍五入到第一个小数位
    Serial.print("\n");

    // Serial.print("SW: ");
    // Serial.print(sw);
    // Serial.print("\n\n");
    // sending_data.isButtonPressed = sw;  //这一块就是上面定义的那个列表，自己改一下呗
    sending_data.a1 = a1_degree;
    sending_data.a2 = a2_degree;
    sending_data.a3 = a3_degree;
    sending_data.a4 = a4_degree;
    sending_data.a5 = a5_degree;

    esp_err_t result =
        esp_now_send(0, (uint8_t *)&sending_data, sizeof(data_structure));

    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }
    delay(500);
}



//接收端
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
