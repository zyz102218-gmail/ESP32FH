#include "WiFi.h"
#include "esp_now.h"
#include "Arduino.h"
#include "ArduinoJson.h"

const char *ssid = "TOUTAIS_PC";
const char *password = "hzr20020803";
int HandsA1Max, HandsA1Min;
int HandsA2Max, HandsA2Min;
int HandsA3Max, HandsA3Min;
int HandsA4Max, HandsA4Min;
int HandsA5Max, HandsA5Min;
uint8_t broadcastAddress1[] = {0x94, 0xB9, 0x7E, 0xDE, 0xE2, 0xF4};
// if you have other receivers you can define in the following
// uint8_t broadcastAddress2[] = {0xFF, , , , , };
// uint8_t broadcastAddress3[] = {0xFF, , , , , };
// 接收端的mac：94:B9:7E:E9:43:A8

// SoftAP基础设置
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
typedef struct data_structure //手指数据结构
{
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
} data_structure;
data_structure RecingData;
void OnDataRecieve(const uint8_t *mac_addr, const uint8_t *inComingData, int data_len)
{
    memcpy(&RecingData, inComingData, sizeof(RecingData));
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);                           //设置为AP模式
    WiFi.softAP(ssid, password);                  //初始化WiFi,开启softAP模式，大概好像也许会给自己分配一个192.168.4.1
    WiFi.softAPConfig(local_IP, gateway, subnet); //初始化softAP，此时esp的WiFi部分已经初始化完成
    // WiFi.begin(ssid, password); //连接到WiFi

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_recv_cb_t(OnDataRecieve); //注册回调处理函数

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
    a1_degree = a1_degree * (5.0 / 4095.0);
    a2_degree = a2_degree * (5.0 / 4095.0);
    a3_degree = a3_degree * (5.0 / 4095.0);
    a4_degree = a4_degree * (5.0 / 4095.0);
    a5_degree = a5_degree * (5.0 / 4095.0);

    a1_degree = map(a1_degree, 2000, 4095, 15, 115);
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

    // esp_err_t result =
    //     esp_now_send(0, (uint8_t *)&sending_data, sizeof(data_structure));

    delay(1500);
}
