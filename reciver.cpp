// code from https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/

/*
 * Code by Zirui Hu & Dill Zhu
 * BUAA Physics
 * Date: 20220207
 * 数据接收端
 * 目前差机械手控制部分
 *
 */

//接受端
#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>

//定义softAP相关设置
const char *ssid = "DillTest";
const char *password = "12345678";
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// 定义数据接收结构体
// ESP-NOW协议使用
// a1是0-4095之间的数
typedef struct struct_message
{
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
} struct_message;
//初始化一个结构体，用于接收数据
struct_message incomingReadings;
//初始化一个String，用于文字信息提示 这个字符串是展示在网页上的提示文本，一定要注意适时修改
String InfoStr = "";
//初始化一个flag，标识当前程序进展顺序
// 1 -> 正在采集最大
// 2 -> 正在采集最小
// 3 -> 初始化完成，进行相关工作，开始映射
int flag = 1;
//创建结构体，用于存储数据最大和最小静态值
struct FingerStaticData
{
    int Finger1;
    int Finger2;
    int Finger3;
    int Finger4;
    int Finger5;
};
//结构题这一块……之前出了个野指针，导致panic
//初始化结构体，最大：
FingerStaticData *MAX = new FingerStaticData;
//初始化结构体，最小：
FingerStaticData *MIN = new FingerStaticData;
//定义一个mapping函数
int mapping(const int &data, const int &min, const int &max)
{
    if (data <= min)
    {
        return 0;
    }
    else if (data >= max)
    {
        return 180;
    }
    else
    {
        float len = max - min;
        float addV = data - min;
        return 180.0f * (addV / len);
    }
}
//初始化一个用于确认按钮是否被按下的量
// 0 未被按下
// 1 已被按下
int ButtonClicked = 0;
//用json创建一个fingers，存储数据，用于ajax刷新页面
JSONVar fingers;

// AsyncWebServer server(80); //创建异步服务器
AsyncWebServer *server = new AsyncWebServer(80); //在80端口创建异步服务器 指针形式
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
    //这一部分未来会改成mapping
    if (flag >= 3) //如果初始化过程，最大最小采集全部完成
    {              //执行mapping
        fingers["a1"] = mapping(incomingReadings.a1, MIN->Finger1, MAX->Finger1);
        fingers["a2"] = mapping(incomingReadings.a2, MIN->Finger2, MAX->Finger2);
        fingers["a3"] = mapping(incomingReadings.a3, MIN->Finger3, MAX->Finger3);
        fingers["a4"] = mapping(incomingReadings.a4, MIN->Finger4, MAX->Finger4);
        fingers["a5"] = mapping(incomingReadings.a5, MIN->Finger5, MAX->Finger5);
    }
    else
    { //没有初始化，没有角度数据
        fingers["a1"] = "--";
        fingers["a2"] = "--";
        fingers["a3"] = "--";
        fingers["a4"] = "--";
        fingers["a5"] = "--";
    }
    fingers["text"] = InfoStr; //一定要注意这部分的更新
    String jsonString = JSON.stringify(fingers);

    //收集所有接收到的数据后jsonString变量，将该信息作为事件 （"new_readings"）.
    // millis的意义相当于delay函数
    // 其实并不是，是打一个时间戳作为发送ID，用于标识这个数据是不是被重复发送的
    events.send(jsonString.c_str(), "new_readings", millis());
}

#ifndef ___HTML //定义网页
#define ___HTML
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
        <p><span id="text_info">233</span></p>
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
        document.getElementById("text_info").innerHTML = obj.text;
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
#endif

void setup()
{
    //初始化串口，设置波特率为115200
    Serial.begin(115200);

    //其实softAP模式相当于AP+DHCP服务器形式，自己分配IP
    //设置WIFI工作在softAP模式，不开启STA连接其他WiFi
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    //串口输出debug信息
    Serial.println();
    Serial.print("The SSID is: ");
    Serial.println(ssid);
    Serial.print("The password is: ");
    Serial.println(password);
    Serial.print("The station's IP address is: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("MAC Address is: ");
    Serial.println(WiFi.macAddress());
    esp_now_init();

    //初始化esp now
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else
    {
        Serial.println("ESP-NOW initialied! Sueecessed");
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
    //往上，都不知道是什么意思，为什么要有这个？（这个确实没用，只是调试网页，确定其运行情况方便）
    //设置网页交互模块，定义按下按钮后发送的GET操作如何处理（目前是在串口打印确认接收的信息）
    server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
               {
               String inputMessage;
               if (request->hasParam("input"))
               {
                 inputMessage = request->getParam("input")->value();
                 ButtonClicked = 1; //设置按钮被按下
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

//每5000ms发送一个ping，检查服务器运行状况
void loop()
{
    if (flag == 1) //采集最大值，最小值，采集过程异常处理
    {
        //采集最大值
        InfoStr = "请保持到最大水平，后按下按钮";
        if (ButtonClicked == 1)
        {
            MAX->Finger1 = incomingReadings.a1;
            MAX->Finger2 = incomingReadings.a2;
            MAX->Finger3 = incomingReadings.a3;
            MAX->Finger4 = incomingReadings.a4;
            MAX->Finger5 = incomingReadings.a5;
            ButtonClicked = 0;
            flag = 2;
        }
        //记录最大值
    }
    else if (flag == 2)
    {
        //采集最小值
        InfoStr = "请保持到最小水平，后按下按钮";
        if (ButtonClicked == 1)
        {
            MIN->Finger1 = incomingReadings.a1;
            MIN->Finger2 = incomingReadings.a2;
            MIN->Finger3 = incomingReadings.a3;
            MIN->Finger4 = incomingReadings.a4;
            MIN->Finger5 = incomingReadings.a5;
            ButtonClicked = 0;
            //下面开始检查数据是否有问题
            if (MAX->Finger1 <= MIN->Finger1 || MAX->Finger2 <= MIN->Finger2 ||
                MAX->Finger3 <= MIN->Finger3 || MAX->Finger4 <= MIN->Finger4 ||
                MAX->Finger5 <= MIN->Finger5)
            {
                //至少有一个手指存在问题
                flag = 1;
                Serial.println("Data is inpossible to wave. At least one finger's max data is smaller than minium data");
                Serial.print("MIN Data : ");
                Serial.print(MIN->Finger1);
                Serial.print(" ");
                Serial.println(MIN->Finger2);
                Serial.print(MIN->Finger3);
                Serial.print(" ");
                Serial.println(MIN->Finger5);
                Serial.print("MAX Data: ");
                Serial.print(MAX->Finger1);
                Serial.print(" ");
                Serial.println(MAX->Finger2);
                Serial.print(MAX->Finger3);
                Serial.print(" ");
                Serial.print(MAX->Finger4);
                Serial.print(" ");
                Serial.println(MAX->Finger5);
                Serial.println("Restart getting information");
                InfoStr = "初始化失败，按下按钮重新开始"; //推送数据到网页
                flag = 1;
            }
            else
            { //数据没有问题，开始正常工作
                flag = 3;
            }
        }
        //复位
    }

    else
    {
        InfoStr = "初始化工作完成，可以开始工作";
    }
    //数据初始化过程结束
    static unsigned long lastEventTime = millis();
    static const unsigned long EVENT_INTERVAL_MS = 5000;
    if ((millis() - lastEventTime) > EVENT_INTERVAL_MS)
    {
        events.send("ping", NULL, millis());
        lastEventTime = millis();
    }
    // Serial.print("Button status: ");
    // Serial.println(ButtonClicked);
    // Serial.print("Flag: ");
    // Serial.println(flag);
    delay(200);
}
