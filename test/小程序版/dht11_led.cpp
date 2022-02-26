// #include <ESP8266WiFi.h>//默认，加载WIFI头文件
// #include "PubSubClient.h"//默认，加载MQTT库文件
// #include <SimpleDHT.h>//dht11库文件
// #include <Arduino.h>

const char* ssid = "newhtc";                  //修改，修改为你的路由的WIFI名字
const char* password = "qq123456";           //修改为你的WIFI密码
const char* mqtt_server = "bemfa.com";       //默认，MQTT服务器地址
const int mqtt_server_port = 9501;          //默认，MQTT服务器端口
#define ID_MQTT  "4d9ec352e0376f2110a0c601a2857225"   //mqtt客户端ID，修改为你的开发者密钥
const char*  topic = "led002";                       //Led主题名字，可在巴法云控制台自行创建，名称随意
const char * dhttopic = "temp004";                 //温湿度主题名字，可在巴法云mqtt控制台创建
int pinDHT11 = D4;                         //dht11传感器引脚输入
int B_led = D5;                           //控制的led引脚
long timeval = 3 * 1000;                  //上传的传感器时间间隔，默认3秒



String ledstatus = "off";//led状态默认off
long lastMsg = 0;//时间戳
SimpleDHT11 dht11(pinDHT11);//dht11初始化
WiFiClient espClient;
PubSubClient client(espClient);//mqtt初始化


//灯光函数及引脚定义
void turnOnLed();
void turnOffLed();



void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String Mqtt_Buff = "";
  for (int i = 0; i < length; i++) {
    Mqtt_Buff += (char)payload[i];
  }
  Serial.print(Mqtt_Buff);
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (Mqtt_Buff == "on") {//如果接收字符on，亮灯
    turnOnLed();//开灯函数

  } else if (Mqtt_Buff == "off") {//如果接收字符off，亮灯
    turnOffLed();//关灯函数
  }
  Mqtt_Buff = "";
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID_MQTT)) {//连接mqtt
      Serial.println("connected");
      client.subscribe(topic);//修改，修改为你的主题
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(B_led, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);
  digitalWrite(B_led, HIGH);
}
void loop() {
  if (!client.connected()) {//判断mqtt是否连接
    reconnect();
  }
  client.loop();//mqtt客户端


  long now = millis();//获取当前时间戳
  if (now - lastMsg > timeval) {//如果达到3s，进行数据上传
    lastMsg = now;


    // read without samples.
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
      return;
    }


    String  msg = "#" + (String)temperature + "#" + (String)humidity + "#" + ledstatus; //数据封装#温度#湿度#开关状态#
    client.publish(dhttopic, msg.c_str());//数据上传
  }



}

//打开灯泡
void turnOnLed() {
  ledstatus = "on";
  Serial.println("turn on light");
  digitalWrite(B_led, LOW);
}
//关闭灯泡
void turnOffLed() {
  ledstatus = "off";
  Serial.println("turn off light");
  digitalWrite(B_led, HIGH);
}


//原文是ino文件，强行改后缀的
