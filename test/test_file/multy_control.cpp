// #include <ESP32Servo.h>

// Servo servo1;
// Servo servo2;
// Servo servo3;
// Servo servo4;

// int servopin1 = 1;
// int servopin2 = 2;
// int servopin3 = 3;
// int servopin4 = 4;

// int pos = 0;

// void setup() {
//   Serial.begin(115200);
//   servo1.setPeriodHertz(50);
//   servo2.setPeriodHertz(50);
//   servo3.setPeriodHertz(50);
//   servo4.setPeriodHertz(50);
// }

// void loop() {


//   if (Serial.available() > 2) {serialA = Serial.read();Serial.println(serialA);}
//   {
//     unsigned int servopos = Serial.read();
//     unsigned int servopos1 = Serial.read();
//     unsigned int realservo = (servopos1 *256) + servopos; 
//     Serial.println(realservo); 
    
//     if (realservo >= 1000 && realservo <1180){
//     int servo1 = realservo;
//     servo1 = map(servo1, 1000,1180,0,180);
//     myservo1.write(servo1);
//     Serial.println("servo 1 ON");
//     delay(10);

//     }
    
//     if (realservo >=2000 && realservo <2180){
//       int servo2 = realservo;
//       servo2 = map(servo2,2000,2180,0,180);
//       myservo2.write(servo2);
//       Serial.println("servo 2 On");
//       delay(10);
      
//     }
    
//     if (realservo >=3000 && realservo < 3180){
//       int servo3 = realservo;
//       servo3 = map(servo3, 3000, 3180,0,180);
//       myservo3.write(servo3);
//       Serial.println("servo 3 On");
//       delay(10);
//     }
//     if (realservo >=4000 && realservo < 4180){
//       int servo4 = realservo;
//       servo4 = map(servo4, 4000, 4180,0,180);
//       myservo4.write(servo4);
//       Serial.println("servo 4 On");
//       delay(10);
//     }
    
//     if (realservo >=5000 && realservo < 5180){
//       int servo5 = realservo;
//       servo5 = map(servo5, 5000, 5180,0,180);
//       myservo5.write(servo5);
//       Serial.println("servo 5 On");
//       delay(10);
//     }
    
//     if (realservo >=6000 && realservo < 6180){
//       int servo6 = realservo;
//       servo6 = map(servo6, 6000, 6180,0,180);
//       myservo6.write(servo6);
//       Serial.println("servo 6 On");
//       delay(10);
//     }
// }



// #include <ESP32Servo.h>

// const int servoCount = 4;// how many servo 
// static const int servosPins[servoCount] = {12, 14, 27, 26}; // define pins here

// Servo servos[servoCount];//do not change

// //初始化角度，把所有的角度调整一下
// void setServos(int degrees) {
//     for(int i = 0; i < servoCount; ++i) {
//         servos[i].write((degrees + (35 * i)) % 180);
//     }
// }

// void setup() {
//     Serial.begin(115200);
// 	Serial.print("let's begain");

// }



//发射端
#include <WiFi.h>
#include <esp_now.h>
#include "Arduino.h"

uint8_t broadcastAddress1[] = {0x ? ?, 0x ? ?, 0x ? ?, 0x ? ?, 0x ? ?, 0x ? ? };
// if you have other receivers you can define in the following
// uint8_t broadcastAddress2[] = {0xFF, , , , , };
// uint8_t broadcastAddress3[] = {0xFF, , , , , };

typedef struct data_structure {
    int x;
    int y;
    bool isButtonPressed;
} data_structure;

data_structure sending_data;
const int JOY_STICK_VRX = 34;
const int JOY_STICK_VRY = 35;
const int PUSH_BUTTON   = 18;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    char macStr[18];
    Serial.print("Packet to: ");
    // Copies the sender mac address to a string
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
             mac_addr[5]);
    Serial.print(macStr);
    Serial.print(" send status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
    pinMode(JOY_STICK_VRX, INPUT);
    pinMode(JOY_STICK_VRY, INPUT);
    pinMode(PUSH_BUTTON, INPUT_PULLUP);
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);

    if(esp_now_init() != ESP_OK) {
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

    if(esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}


void loop() {
    float x_axis = analogRead(JOY_STICK_VRX);
    float y_axis = analogRead(JOY_STICK_VRY);
    int sw = digitalRead(PUSH_BUTTON);
    int axis_max = 4095;
    // Convert joy stick value from -45 to 45，这个自己视情况调一下
    x_axis = (x_axis - axis_max / 2) / 22.76;
    y_axis = (y_axis - axis_max / 2) / 22.76;

    Serial.print("X_axis: ");
    Serial.print(x_axis, 0); // 四舍五入到第一个小数位
    Serial.print("\n");
    Serial.print("Y-axis: ");
    Serial.print(y_axis, 0); // 四舍五入到第一个小数位
    Serial.print("\n");
    Serial.print("SW: ");
    Serial.print(sw);
    Serial.print("\n\n");
    sending_data.isButtonPressed = sw;  //这一块就是上面定义的那个列表，自己改一下呗
    sending_data.x = x_axis;
    sending_data.y = y_axis;
    
    esp_err_t result =
        esp_now_send(0, (uint8_t *)&sending_data, sizeof(data_structure));

    if(result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
    delay(100);
}






//接收端
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

Servo x_servo;  
Servo y_servo;

int x_pos = 0;
int y_pos = 0;    

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int y_servoPin = 18;
int x_servoPin = 19;


// receive data structure,Must match the sender structure
typedef struct data_structure {
  int x;
  int y;
  bool isButtonPressed;
} data_structure;

// Create a struct_message called receivedData
data_structure receivedData;

// callback function that will be executed when ESP-NOW data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x: ");
  Serial.println(receivedData.x);
  Serial.print("y: ");
  Serial.println(receivedData.y);
  Serial.print("isButtonPressed: ");
  Serial.println(receivedData.isButtonPressed);
  Serial.println();
  y_servo.write(90+int(receivedData.x));
  x_servo.write(90+int(receivedData.y));

}

void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  x_servo.setPeriodHertz(50);    // standard 50 hz servo
  x_servo.attach(x_servoPin, 100, 2400);
  y_servo.setPeriodHertz(50);    // standard 50 hz servo
  y_servo.attach(y_servoPin, 100, 2400);
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {

}

