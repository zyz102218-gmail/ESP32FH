// code from https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/

#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

//记得改一下mac地址
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
} struct_message;

struct_message myData;

//创建一些辅助计时器变量10000ms发布一次
unsigned long previousMillis = 0; // Stores last time temperature was published
const long interval = 10000;      // Interval at which to publish sensor readings
//老实说我不知道上面的有什么用

//回调函数，发送成功的反馈
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin();
    //初始化ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    //注册回调函数
    esp_now_register_send_cb(OnDataSent);
}

void loop()
{
    // ESP-NOW发送消息结构
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }
}