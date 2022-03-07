// code from https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/
/*

 * Code by Zirui Hu & Dill Zhu
 * BUAA Physics
 * Date: 20220225
 * Latest commit: 20220307
 * 数据发送端
 * 目前需要解决的问题是ESP-NOW的发送/接收问题，仍然需要解决
 * 目前接收不成功，发送端反馈未发送，接受端接收函数没有被执行
 * 原因查明：没有添加peer
 */
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

/* Important! The ADC2 cannot be used when Wi-Fi function is enabled.
 * Thus, if we wanted to enable ESP-Now to transform information,
 * we cannot use ADC2 to generate information.
 * GPIO for ADC1 is 36, 39, 34, 35, 32, 33
 */

/* As a matter of fact, the ADC supports serval modes,
 * which will change its resolution. On default, it works on 12bits.
 *
 */
//记得改一下mac地址
uint8_t broadcastAddress[] = {0x78, 0xE3, 0x6D, 0x64, 0x1A, 0xD9};
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
const char *ssid = "DillTest";
const char *password = "12345678";
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
    WiFi.begin(ssid, password);

    //初始化ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else
    {
        Serial.println("ESP-Now initializiation Successed!");
    }
    esp_now_peer_info peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    //注册回调函数
    esp_now_register_send_cb(OnDataSent);
}
//读取ADC数值
void ReadFromADCs()
{
    myData.a1 = int(analogRead(36));
    myData.a2 = int(analogRead(39));
    myData.a3 = int(analogRead(35));
    myData.a4 = int(analogRead(32));
    myData.a5 = int(analogRead(34));
}
void loop()
{
    // ESP-NOW发送消息结构
    ReadFromADCs();
    // Serial.println("------------------");
    // Serial.print("Tag is: ");
    // Serial.println(millis());
    // Serial.println(myData.a1);
    // Serial.println(myData.a2);
    // Serial.println(myData.a3);
    // Serial.println(myData.a4);
    // Serial.println(myData.a5);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
        Serial.println(result); //报0x3069(12193) 查表得知找不到Peer
    }

    //需要删除
    delay(500);
}