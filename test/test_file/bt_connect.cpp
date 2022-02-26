// #include <esp_now.h>
// #include <WiFi.h>
// #include <Wire.h>

// // 通讯mac，输入接受端
// uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// // 存储变量用来发送
// float send_01;
// float send_02;
// float send_03;
// float send_04;
// float send_05;
// float send_06;

// // 创建变量用来存储接受
// float incoming_01;
// float incoming_02;
// float incoming_03;
// float incoming_04;
// float incoming_05;
// float incoming_06;

// // 如果读数成功
// String success; //这里的success也可以用其他的，只是一个代表，下面有success=

// //Structure example to send data
// //Must match the receiver structure
// typedef struct struct_message {
//     float temp;
//     float hum;
//     float pres;
// } struct_message;

// // Create a struct_message called BME280Readings to hold sensor readings
// struct_message BME280Readings;
// // Create a struct_message to hold incoming sensor readings
// struct_message incomingReadings;

// // 发送端发射的数据
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   //数据回调，如果消息传递成功，则返回0，然后根据状态print
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//   if (status ==0){
//     success = "Delivery Success :)";
//   }
//   else{
//     success = "Delivery Fail :(";
//   }
// }

// // 接受端接受的数据
// void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
//     //这是一个存储的位置
//   memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
//   //打印接受的数据
//   Serial.print("Bytes received: ");
//   Serial.println(len);
//   //存储数据
//   incomingTemp = incomingReadings.temp;
//   incomingHum = incomingReadings.hum;
//   incomingPres = incomingReadings.pres;
// }

// void setup() {
//   // Init Serial Monitor
//   Serial.begin(115200);
//   // Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   // Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Once ESPNow is successfully Init, we will register for Send CB to
//   // get the status of Trasnmitted packet
//   esp_now_register_send_cb(OnDataSent);

//   // Register peer
//   esp_now_peer_info_t peerInfo;
//   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   peerInfo.channel = 0;
//   peerInfo.encrypt = false;

//   // Add peer
//   if (esp_now_add_peer(&peerInfo) != ESP_OK){
//     Serial.println("Failed to add peer");
//     return;
//   }
//   // Register for a callback function that will be called when data is received
//   esp_now_register_recv_cb(OnDataRecv);
// }

// void loop() {
//   getReadings();

//   // 更新数据
//   BME280Readings.temp = temperature;
//   BME280Readings.hum = humidity;
//   BME280Readings.pres = pressure;

//   // 用ESP-NOW发送数据
//   esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &BME280Readings, sizeof(BME280Readings));

//   if (result == ESP_OK) {
//     Serial.println("Sent with success");
//   }
//   else {
//     Serial.println("Error sending the data");
//   }
//   //更新
//   updateDisplay();
//   delay(10000);
// }

// void getReadings(){
// //   temperature = bme.readTemperature();
// //   humidity = bme.readHumidity();
// //   pressure = (bme.readPressure() / 100.0F);
// }

// void updateDisplay(){

// //   display.clearDisplay();
// //   display.setTextSize(1);
// //   display.setTextColor(WHITE);
// //   display.setCursor(0, 0);
// //   display.println("INCOMING READINGS");
// //   display.setCursor(0, 15);
// //   display.print("Temperature: ");
// //   display.print(incomingTemp);
// //   display.cp437(true);
// //   display.write(248);
// //   display.print("C");
// //   display.setCursor(0, 25);
// //   display.print("Humidity: ");
// //   display.print(incomingHum);
// //   display.print("%");
// //   display.setCursor(0, 35);
// //   display.print("Pressure: ");
// //   display.print(incomingPres);
// //   display.print("hPa");
// //   display.setCursor(0, 56);
// //   display.print(success);
// //   display.display();

//   // Display Readings in Serial Monitor
//   Serial.println("INCOMING READINGS");
//   Serial.print("Temperature: ");
//   Serial.print(incomingReadings.temp);
//   Serial.println(" ºC");
//   Serial.print("Humidity: ");
//   Serial.print(incomingReadings.hum);
//   Serial.println(" %");
//   Serial.print("Pressure: ");
//   Serial.print(incomingReadings.pres);
//   Serial.println(" hPa");
//   Serial.println();
// }

include "BluetoothSerial.h"

    BluetoothSerial SerialBT;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("Client Connected");
  }
}

void setup()
{
  Serial.begin(115200);
  SerialBT.register_callback(callback);

  if (!SerialBT.begin("ESP32"))
  {
    Serial.println("An error occurred initializing Bluetooth");
  }
  else
  {
    Serial.println("Bluetooth initialized");
  }
}

void loop() {}