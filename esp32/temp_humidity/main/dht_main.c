#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "dht11.h"
#include "mqtt_client.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h" //light weight ip packets error handling
#include "lwip/sys.h"
#include "cJSON.h"

#define DHT_PIN  (gpio_num_t) _DHT_PIN

#define WIFI_SSID "MERCUSYS_B255"
#define WIFI_PSWD "65388518"

#define CONFIG_BROKER_URL "mqtt://192.168.1.10:1888"

#define DEVICE_TAG "ESP32-DHT11"

const int CONNECTED_BIT = BIT0;

const char *ssid = WIFI_SSID;
const char *pass = WIFI_PSWD;
int retry_num=0;
esp_mqtt_client_handle_t  client;

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data){
    if(event_id == WIFI_EVENT_STA_START)
    {
        printf("WIFI CONNECTING....\n");
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED)
    {
        printf("WiFi CONNECTED\n");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        printf("WiFi lost connection\n");
        if(retry_num<10){esp_wifi_connect();retry_num++;printf("Retrying to Connect...\n");}
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        printf("Wifi got IP...\n\n");
    }
}

void wifi_connection()
{
    //                          s1.4
    // 2 - Wi-Fi Configuration Phase
    esp_netif_init();
    esp_event_loop_create_default();     // event loop                    s1.2
    esp_netif_create_default_wifi_sta(); // WiFi station                      s1.3
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); //
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
            .sta = {
                    .ssid = "",
                    .password = "",

            }

    };
    strcpy((char*)wifi_configuration.sta.ssid, ssid);
    strcpy((char*)wifi_configuration.sta.password, pass);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    // 3 - Wi-Fi Start Phase
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    // 4- Wi-Fi Connect Phase
    esp_wifi_connect();
    printf( "wifi_init_softap finished. SSID:%s  password:%s",ssid,pass);

}

void mqtt_connection() {
    esp_mqtt_client_config_t mqtt_cfg = {
            .broker.address.uri = CONFIG_BROKER_URL,
            .credentials.client_id = "ESP_32_DHT11",
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
//    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void publishValues() {
    //TODO FIXME считывает данные неправильно (проблема с датчиком или чтением???)
    cJSON *data;
    data = cJSON_CreateObject();
    cJSON_AddNumberToObject(data, "temperature", (int)get_temperatureC());
    cJSON_AddNumberToObject(data, "humidity", (int)get_humidity());
    cJSON_AddStringToObject(data, "deviceName", DEVICE_TAG);
    char* dataJsonString = cJSON_Print(data);
    esp_mqtt_client_publish(client, "esp_data", dataJsonString, 0, 0, 0);
    cJSON_free(dataJsonString);
    cJSON_Delete(data);
}

void app_main(void) {
    nvs_flash_init();
    wifi_connection();
    mqtt_connection();
    gpio_num_t dht_pin = DHT_PIN;
    gpio_set_level(dht_pin, HIGH);
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (true) {
        STATE state = read_dht(dht_pin);
        switch (state) {
            case SUCCESS:
                publishValues();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                break;
            case ERROR_TIMEOUT:
                ESP_LOGE(TAG, "ERROR BY TIMEOUT");
                break;
            case ERROR_CHECKSUM:
                ESP_LOGE(TAG, "ERROR BY CHECKSUM");
        }
    }
}