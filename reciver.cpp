// code from https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/

//接受端
#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>

// Replace with your network credentials (STATION)
const char *ssid = "DillTest";
const char *password = "12345678";
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// Structure example to receive data
// Must match the sender structure
// a1是0-4095之间的数
typedef struct struct_message
{
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
} struct_message;

struct_message incomingReadings;

//用json创建一个fingers，存储数据，用于ajax刷新页面
JSONVar fingers;

// AsyncWebServer server(80); //创建异步服务器
AsyncWebServer *server = new AsyncWebServer(80); //创建异步服务器
AsyncEventSource events("/events");              //创建事件源 /events

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    // Copies the sender mac address to a string
    char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
    //打印发件人mac，这个不需要可以删除
    //不过反正是在终端打印，跟实际的传输一点关系都没有

    //复制传入数据变量进入传入读取结构变量
    //这个地方是不是因为incomingdata不是字符串或者转译啥的啊
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

    //使用收到的信息创建一个 JSON 字符串变量
    fingers["a1"] = incomingReadings.a1;
    fingers["a2"] = incomingReadings.a2;
    fingers["a3"] = incomingReadings.a3;
    fingers["a4"] = incomingReadings.a4;
    fingers["a5"] = incomingReadings.a5;
    String jsonString = JSON.stringify(fingers);

    //收集所有接收到的数据后jsonString变量，将该信息作为事件 （"new_readings").
    // millis的意义相当于delay函数
    events.send(jsonString.c_str(), "new_readings", millis());

    // Serial.printf("t value: %4.2f \n", incomingReadings.a1);
    // Serial.printf("h value: %4.2f \n", incomingReadings.a2);
    // Serial.printf("t value: %4.2f \n", incomingReadings.a3);
    // Serial.printf("h value: %4.2f \n", incomingReadings.a4);
    // Serial.printf("h value: %4.2f \n", incomingReadings.a5);

    // Serial.println();
}

//构建网页
const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
<html>

<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    /*以下为按钮风格化代码，抄写的*/
    .btn {
      width: 150px;
      height: 50px;
      border: none;
      background-color: green;
      line-height: 50px;
      font-size: 30px;
      text-align: center;
      color: white;
      border-radius: 50px;
    }

    .btn:focus {
      outline: none;
    }

    .btn-primary {
      background-color: blue;
    }

    .btn-success {
      background-color: green;
    }

    .btn-warning {
      background-color: yellow;
    }

    .btn-danger {
      background-color: red;
    }

    /*上述为按钮风格化代码，抄写的*/
    html {
      font-family: Arial;
      display: inline-block;
      text-align: center;
    }

    p {
      font-size: 1.2rem;
    }

    body {
      margin: 0;
    }

    .topnav {
      overflow: hidden;
      background-color: #2f4468;
      color: white;
      font-size: 1.2rem;
    }

    .content {
      padding: 20px;
    }

    .card {
      background-color: white;
      box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    }

    .cards {
      max-width: 700px;
      margin: 0 auto;
      display: grid;
      grid-gap: 2rem;
      grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    }

    .reading {
      font-size: 2rem;
    }

    .packet {
      color: #bebebe;
    }

    .card.temperature {
      color: #fd7e14;
    }

    .card.humidity {
      color: #1b78e2;
    }
  </style>
</head>

<body>
  <div class="topnav">
    <h3>FINGERS DHT RECIEVER</h3>
  </div>

  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> Finger1</h4>
        <p><span class="reading"><span id="t1">Data</span> &deg;</span></p>
        <h4><i class="fas fa-thermometer-half"></i> Finger2</h4>
        <p><span class="reading"><span id="t2">Data</span> &deg;</span></p>
        <h4><i class="fas fa-thermometer-half"></i> Finger3</h4>
        <p><span class="reading"><span id="t3">Data</span> &deg;</span></p>
        <h4><i class="fas fa-thermometer-half"></i> Finger4</h4>
        <p><span class="reading"><span id="t4">Data</span> &deg;</span></p>
        <h4><i class="fas fa-thermometer-half"></i> Finger5</h4>
        <p><span class="reading"><span id="t5">Data</span> &deg;</span></p>
        <p><span id="text">233</span></p>
        <input id="clickbut" type="button" value="Button" class="btn btn-primary">
      </div>
    </div>
  </div>
  <script>
    document.getElementById("clickbut").addEventListener("click", doPost);

    //创建新的events，并发送到页面
    if (!!window.EventSource) {
      var source = new EventSource('/events');

      //实例事件源
      source.addEventListener('open', function (e) {
        console.log("Events Connected");
      }, false);
      source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);

      source.addEventListener('message', function (e) {
        console.log("message", e.data);
      }, false);

      //事件侦听器
      source.addEventListener('new_readings', function (e) {
        console.log("new_readings", e.data);
        var obj = JSON.parse(e.data);
        document.getElementById("t1").innerHTML = obj.a1;
        document.getElementById("t2").innerHTML = obj.a2;
        document.getElementById("t3").innerHTML = obj.a3;
        document.getElementById("t4").innerHTML = obj.a4;
        document.getElementById("t5").innerHTML = obj.a5;
        document.getElementById("text").innerHTML = obj.text;
      }, false);
    }

    //按下按钮后发送请求数据
    function doPost() {
      var httpRequest = new XMLHttpRequest(); //创建需要发送的XML对象
      //httpRequest.open("GET", "/update?output=clicked", true);
      //httpRequest.setRequestHeader("Content-type", "text/plain");
      httpRequest.open("GET", "/update?input=1", true);
      httpRequest.send();
      //log("233");
    }
  </script>
</body>

</html>)rawliteral";

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    Serial.println();
    Serial.print("The SSID is: ");
    Serial.println(ssid);
    Serial.print("The password is: ");
    Serial.println(password);
    Serial.print("The station's IP address is: ");
    Serial.println(WiFi.softAPIP());

    //初始化esp now
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    //注册回调函数，每个数据包执行一次
    esp_now_register_recv_cb(OnDataRecv);

    //访问ip时，发送存储在/index_html的变量构建这个网页
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send_P(200, "text/html", index_html); });

    //往后而至
    //在服务器设置事件源，即要被监听的对象
    events.onConnect([](AsyncEventSourceClient *client)
                     {
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());

    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000); });
    //往上，都不知道是什么意思，为什么要有这个？
    server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
               {
               String inputMessage;
               if (request->hasParam("input"))
               {
                 inputMessage = request->getParam("input")->value();
               }
               else
               {
                 inputMessage = "no message";
               }
               Serial.print("Messages from request: ");
               Serial.println(inputMessage);
               request->send(200, "text/plain", "OK"); });

    server->addHandler(&events);

    server->begin();
}

int i = 0;
int a[] = {1, 2, 3, 4, 5, 6};

//每5000ms发送一个ping，检查服务器运行状况
void loop()
{
    //刷新测试用，后期需要删除
    i++;
    fingers["a1"] = a[i];
    fingers["a2"] = a[i];
    fingers["a3"] = a[i];
    fingers["a4"] = a[i];
    fingers["a5"] = a[i];
    fingers["text"] = "请按下按钮采集初始值";
    String jsonString = JSON.stringify(fingers);
    events.send(jsonString.c_str(), "new_readings", millis());
    static unsigned long lastEventTime = millis();
    static const unsigned long EVENT_INTERVAL_MS = 5000;
    if ((millis() - lastEventTime) > EVENT_INTERVAL_MS)
    {
        events.send("ping", NULL, millis());
        lastEventTime = millis();
    }
    if (i >= 5)
        i = 0;
    //延时，防止爆内存
    delay(100);
}
