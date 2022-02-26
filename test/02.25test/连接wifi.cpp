#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

const char *ssid = "";         // wifi账号
const char *password = "";     // wifi密码
const char *mqttServer = "";   // mqtt服务器
const int mqttPort = ;         //端口
const char *mqttUser = "";     // mqtt账号
const char *mqttPassword = ""; // mqtt密码

//
String Temp = "temp";
String Hum = "hum";
char str[60];
char str1[20];
char str2[20];
//

WiFiClient espClient;           // 定义wifiClient实例
PubSubClient client(espClient); // 定义PubSubClient的实例

void setup()
{

    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");

    client.setServer(mqttServer, mqttPort); // client 客户端的意思

    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (client.connect(topic, mqttUser, mqttPassword))
        {

            Serial.println("connected");
        }
        else
        {

            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.publish(topic, "Hello from ESP32"); //填写主题
}

void loop()
{
    DHT.read(DHT11_PIN);
    String temp = String(DHT.temperature);
    String humidity1 = String(DHT.humidity);

    sprintf(str, "{\"temp\":%s,\"hum\":%s}", temp, humidity1);

    Serial.println(str);
    client.publish(topic, str); //填写主题

    delay(10000);
    client.loop();
}
