#include "WiFi.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

const char *ssid = "TOUTAIS_PC";
const char *password = "hzr20020803";

const int hands_a1 = 32;     //拇指

AsyncWebServer server(80); // 创建一个异步网络服务器端口80上的对象。

String read_First_finger()
{
  float a = analogRead(hands_a1);
  if (isnan(a))
  {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    Serial.println(a);
    return String(a;
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Machine hands</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">First finger</span> 
    <span id="first_finger">%FIRST%</span>
    <sup class="units">&deg;C</sup>
  </p>
</body>
</html> 
)rawliteral";

// 创建processor()函数，这将用实际的温度和湿度值替换 HTML 文本中的占位符。
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "FIRST")
  {
    return read_First_finger();
  }

  return String();
}
//寻找占位符并进行相应替换


void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");

  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  // 处理 Web 服务器
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  server.on("/first_finger", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", read_First_finger().c_str()); });

  server.begin();
}

void loop()
{
}