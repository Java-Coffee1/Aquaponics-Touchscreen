#include <Arduino.h>
#include "rgb_lcd_port.h"
#include "gt911.h"
#include "lvgl_port.h"
#include <demos/lv_demos.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "esp_task_wdt.h"
#include "ui.h"
#include "touch_ui.h"
#include "lvgl.h"
#include "get_sensor_data.h"
#include "LittleFS.h"
#include "sensor_setup.h"
#include "data_coms/data_communication.h"
// #include "screens.h"

unsigned long previousMillis = 0;
unsigned long interval = 30000; // 30 seconds
unsigned long ui_refresh_PreviousMillis = 0;
unsigned long ui_refresh_Interval = 5000; // 5 seconds

//_______________________________ ESP NOW SETUP ________________________________
// resiver of the data
uint8_t serverAddress[] = {0x88, 0x13, 0xBF, 0x82, 0x39, 0x30};
esp_now_peer_info_t peerInfo;

// void update_sensor_list() {
//     get_sensor_data((std::string name, double current, double min, double max) {
//         create_sensor_list(objects.sensor_list, name);
//     });
//     create_sensor_list(objects.sensor_list, array_ofsensor_DATA);
// }
//_______________ WiFi Setup ______________//
const char *ssid = "AquaponicsIOT";
const char *password = "aquadma134!";

String get_sensor_name = "null";

void setup()
{
    Serial.begin(115200);
    if (!LittleFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }
    Serial.println("File Content:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();

    load_sensor_config();

    static esp_lcd_panel_handle_t panel_handle = NULL;
    static esp_lcd_touch_handle_t tp_handle = NULL;
    tp_handle = touch_gt911_init();
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();
    wavesahre_rgb_lcd_bl_on();
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));
    ESP_LOGI(TAG, "Display LVGL demos");

    if (lvgl_port_lock(-1))
    {
        ui_init();
        setup_main_ui();
        lvgl_port_unlock();
    }

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

    // Register peer
    memcpy(peerInfo.peer_addr, serverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Reconnecting to WiFi...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
    }

    // Every 1 second
    if (currentMillis - ui_refresh_PreviousMillis >= ui_refresh_Interval)
    {
        ui_refresh_PreviousMillis = currentMillis;

        Serial.println("Update sensor data");
        // refresh_sensor_data(get_sensor_name);
        two_point_calibration_refresh();
        sensor_hight_calibration_refresh();
    }
    // void outgoing_message(float 2, float 3, float 6.7);
    // Send message via ESP-NOW every 30 seconds
    // LVGL events handle interactions
    // doWork();
    // esp_task_wdt_reset();
}
