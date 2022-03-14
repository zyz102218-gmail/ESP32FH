# ESP32FH

## Introduction

This project is for our own ESP32 based machine hand controlled system. We used liquid metal as sensor in order to get the movement of the hand, which is one of our real hands.
And makes the eletronical hand to move like our real hand.

The project used two ESP32 models. One is for getting the data, the other is for controlling the machine hand, and transform the data to a local webpage.

We hope our code may be helpful for you, just enjoy it.

## Requirements

**IDE:**  
\* Visual Studio Code (The IDE)  
\* PlatformIO  
**PlatformIO Depends:**  
\* Arduino_Json  
\* AsyncTCP-esphome  
\* ESP32Servo  
\* ESPAsync_WiFiManager  
\* ESPAsyncTCP-esphome  
\* ESPASyncWebServer-esphome  
\* PubSubClient  
**Be make sure that you've checked the librities properly,
or you cannot build or flash the program.**

---

## How to use it

Our self-programed code are in the root directory,
which named <code>reciver.cpp</code> and <code>sending.cpp</code>
(Apologize for our poor english ability).
And if you'd like to build&flash the code for sender,
you should copy the <code>sending.cpp</code>
to the src path and rename it to <code>main.cpp</code>.
If you'd like to build another ESP32-board, just replace the <code>reciver.cpp</code> for the <code>sending.cpp</code>.  
**Important** Be sure to adjust the MAC address inside the program to your own ESP32 board, otherwise, the ESP-Now connection cannot be setup
and the program won't work fine.  
 The Web-Dashboard will be present via the ESP32 build-in AP, we have enabled the softAP function, thus, your smartphone, or iPad, PC etc. could get a IP address via the build-in DHCP server. The maxium connection is 4 devices. And you could open <code>http:///192.168.4.1</code> to enter the dashboard.

---

## Thanks and other resources

We have read a lot of code and use them from [randomnerdtutorials](https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/).
Thanks for their kind tutorials and useful code.  
Their article may also help you.