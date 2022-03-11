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
___
## How to use it
Our self-programed code are in the root directory, 
which named <code>reciver.cpp</code> and <code>sending.cpp</code> 
(Apologize for our poor english ability). 
And if you'd like to build&flash the code for sender, 
you should copy the <code>sending.cpp</code> 
to the src path and rename it to <code>main.cpp</code>. 
If you'd like to build another ESP32-board, just replace the <code>reciver.cpp</code> for the <code>sending.cpp</code>.
___
## Thanks and other resources
We have read a lot of code and use them from [randomnerdtutorials](https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/). 
Thanks for their kind tutorials and useful code. 
They have saved our a lot of time for surfing and searching tutorials, which, 
well, most of them are useless and waste our time.