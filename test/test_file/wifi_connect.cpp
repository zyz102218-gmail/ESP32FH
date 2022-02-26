// #include "WiFi.h"
// #include "ESPAsyncWebServer.h"
// #include <HTTPClient.h>
// #include <ESPAsyncWebServer.h>
// #include <Wire.h>

// const char* ssid = "ESP32-Access-Point";
// const char* password = "123456789";

// AsyncWebServer server(80);     //在80端口创建异步服务器

// //现在定义几个变量，方便传输,自己改,方便定义

// // String readTemp() {
// //   return String(bme.readTemperature());
// // }

// // String readHumi() {
// //   return String(bme.readHumidity());
// // }

// // String readPres() {
// //   return String(bme.readPressure() / 100.0F);
// // }

// String readPres() {
//   return String(a);   //这里弄一个测量电阻的
// }

// void setup(){
//     Serial.begin(115200);
//     Serial.println();
//     Serial.print("Setting AP (Access Point)…");

//     WiFi.softAP(ssid, password);         //将ESP32设置为具有较早定义的 SSID 名称和密码的接入点。

//     IPAddress IP = WiFi.softAPIP();
//     Serial.print("AP IP address: ");
//     Serial.println(IP);

//     server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send_P(200, "text/plain", readTemp().c_str());
//      });

//     server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send_P(200, "text/plain", readHumi().c_str());
//     });

//     server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send_P(200, "text/plain", readPres().c_str());
//      });
  

//     //下面几行是初始化传感器的，这里我们就要对应到初始我们的舵机
//     // bool status;

//     // status = bme.begin(0x76);  
//     // if (!status) {
//     //     Serial.println("Could not find a valid BME280 sensor, check wiring!");
//     //  while (1);
//     //  }
  
//     // 启动
//     server.begin();
// }



// void loop(){
  
// }