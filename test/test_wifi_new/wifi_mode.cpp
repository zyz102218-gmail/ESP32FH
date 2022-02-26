#include <WiFi.h>
#include <esp_now.h>
#include "Arduino.h"
#include <esp_wifi.h>

// wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
// esp_wifi_init(&config);


// //初始化
// void wifi_init_sta(void)
// {
//     s_wifi_event_group = xEventGroupCreate();

//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_create_default_wifi_sta();
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
//                                                         ESP_EVENT_ANY_ID,
//                                                         &event_handler,
//                                                         NULL,
//                                                         NULL));
//     ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
//                                                         IP_EVENT_STA_GOT_IP,
//                                                         &event_handler,
//                                                         NULL,
//                                                         NULL));
//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = IOT_CORE_ESP_WIFI_SSID,
//             .password = IOT_CORE_ESP_WIFI_PASS,
//           .threshold.authmode = WIFI_AUTH_WPA2_PSK,
//             .pmf_cfg = {
//                 .capable = true,
//                 .required = false
//             },
//         },
//     };
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
//     ESP_ERROR_CHECK(esp_wifi_start() );
//     ESP_LOGI(TAG, "wifi_init_sta finished.");
// }


// //回调函数
// static void event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
//     {
//         esp_wifi_connect();
//     } 
//     else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
//     {
//         if (s_retry_num < IOT_CORE_ESP_MAXIMUM_RETRY) 
//         {
//             esp_wifi_connect();
//             s_retry_num++;
//             ESP_LOGI(TAG, "retry to connect to the AP");
//         } 
//         else 
//         {
//             xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
//         }
//         ESP_LOGI(TAG,"connect to the AP fail");
//     } 
//     else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
//     {
//         ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
//         ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
//         s_retry_num = 0;
//         xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
//     }
// }

// //阻塞等待连接
// void esp_wifi_sta_check_state(void)
// {
//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);


//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) 
//     {
//         xEventGroupClearBits(s_wifi_event_group,WIFI_CONNECTED_BIT);  
//         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
//                  IOT_CORE_ESP_WIFI_SSID, IOT_CORE_ESP_WIFI_PASS);
//     } 
//     else if (bits & WIFI_FAIL_BIT) 
//     {
//         xEventGroupClearBits(s_wifi_event_group,WIFI_FAIL_BIT);       
//         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
//                  IOT_CORE_ESP_WIFI_SSID, IOT_CORE_ESP_WIFI_PASS);
//     } 
//     else 
//     {
//         ESP_LOGE(TAG, "UNEXPECTED EVENT");
//     }
// }



//from here begain


//配置ip地址
int tcpip_adapter_init()

//设置事件回调函数
esp_event_loop_init(event_handler, NULL);

//wifi配置初始化与设置
wifi_init_config_t cfg =
WIFI_INIT_CONFIG_DEFAULT();
esp_wifi_init(&cfg) ;

//wifi模式设置
esp_wifi_set_mode(WIFI_MODE_AP);

//AP配置初始化（SSID，PASSWORD，最大连接数，authmode ）
wifi_config_t ap_config = {
  .ap= {   
         .ssid = CONFIG_APSTA_AP_SSID, 
         .password = CONFIG_APSTA_AP_PASSWORD, 
         .ssid_len = 0,
         .max_connection = 4,  
         .authmode = WIFI_AUTH_WPA_PSK
   }
};
esp_wifi_set_config(WIFI_IF_AP,&ap_config);

//启动WIFI
esp_wifi_start()

//