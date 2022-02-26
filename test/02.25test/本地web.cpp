#include "WiFi.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

const char *ssid = "TOUTAIS_PC";
const char *password = "hzr20020803";

const int hands_a1 = 32;     //拇指
const int hands_a2 = 35;     //食指
const int hands_a3 = 34;     //中指
const int hands_a4 = 39;     //无名指
const int hands_a5 = 36;     //小指

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
    return String(a);
  }
}

String read_Second_finger()
{
  float b = analogRead(hands_a2);
  if (isnan(b))
  {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    Serial.println(b);
    return String(b);
  }
}

String read_Third_finger()
{
  float c = analogRead(hands_a3);
  if (isnan(c))
  {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    Serial.println(c);
    return String(c);
  }
}

String read_Fourth_finger()
{
  float d = analogRead(hands_a4);
  if (isnan(d))
  {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    Serial.println(d);
    return String(d);
  }
}

String read_Fifth_finger()
{
  float e = analogRead(hands_a5);
  if (isnan(e))
  {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else
  {
    Serial.println(e);
    return String(e);
  }
}

//上面有个东西值得注意的是，如果没读取到数据就返回两个破折号--，防止传感器无法读数
//读数以字符串类型返回:return String();

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

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

        h2 {
            font-size: 3.0rem;
        }

        p {
            font-size: 3.0rem;
        }

        .units {
            font-size: 1.2rem;
        }

        .dht-labels {
            font-size: 1.5rem;
            vertical-align: middle;
            padding-bottom: 15px;
        }
    </style>
</head>

<body>
    <h2>ESP32 DHT Server</h2>
    <p>
        <span class="dht-labels">First finger</span>
        <span id="first_finger">%FIRST%</span>

    </p>
    <p>
        <span class="dht-labels">Second finger</span>
        <span id="second_finger">%SECOND%</span>
    </p>
    <p>
        <span class="dht-labels">Third finger</span>
        <span id="third_finger">%THIRD%</span>
    </p>
    <p>
        <span class="dht-labels">Fourth finger</span>
        <span id="fourth_finger">%FOURTH%</span>
    </p>
    <p>

        <span class="dht-labels">Fifth finger</span>
        <span id="fifth_finger">%FIFTH%</span>

    </p>
</body>

<script>
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("first_finger").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/first_finger, true");
        xhttp.send();
    }, 10000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("second_finger").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/second_finger", true);
        xhttp.send();
    }, 10000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("third_finger").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/third_finger", true);
        xhttp.send();
    }, 10000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("fourth_finger").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/fourth_finger", true);
        xhttp.send();
    }, 10000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("fifth_finger").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/fifth_finger", true);
        xhttp.send();
    }, 10000);
</script>
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

  else if (var == "SECOND")
  {
    return read_Second_finger();
  }

  else if (var == "THIRD")
  {
    return read_Third_finger();
  }

  else if (var == "FOURTH")
  {
    return read_Fourth_finger();
  }

  else if (var == "FIFTH")
  {
    return read_Fifth_finger();
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
  server.on("/second_finger", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", read_Second_finger().c_str()); });
  server.on("/third_finger", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", read_Third_finger().c_str()); });
  server.on("/fourth_finger", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", read_Fourth_finger().c_str()); });
  server.on("/fifth_finger", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", read_Fifth_finger().c_str()); });

  //因为是纯文本，所以用字符的形式发送，c_str()

  server.begin();
}

void loop()
{
}