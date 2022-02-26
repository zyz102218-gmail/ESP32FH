#include <esp_now.h>

int test_1 = 32;

void setup()
{
    Serial.begin(115200);
    pinMode(test_1, INPUT);
}

void loop()
{
    float test_2 = analogRead(test_1);
    test_2 = map(test_2, 0, 4095, 0, 5);
    Serial.print("voltage: ");
    Serial.print(test_2, 0); 
}