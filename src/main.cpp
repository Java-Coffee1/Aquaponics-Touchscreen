#include <Arduino.h>
#include "rgb_lcd_port.h"
#include "gt911.h"
#include "lvgl_port.h"
#include <demos/lv_demos.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include "esp_task_wdt.h"
#include "ui.h"
#include "touch_ui.h"
#include "lvgl.h"
#include "get_sensor_data.h"
#include "LittleFS.h"
// #include "screens.h"


const char* ssid = "AquaponicsIOT";
const char* password = "aquadma134!";

std::string sensor_name;
double current_value;
double min_value;
double max_value;
double sensor_hight;;


void wifi_init(){
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Setting as a Wi-Fi Station..");
    }
    Serial.print("Station IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wi-Fi Channel: ");
    Serial.println(WiFi.channel());

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
}

// void update_sensor_list() {
//     get_sensor_data((std::string name, double current, double min, double max) {
//         create_sensor_list(objects.sensor_list, name);
//     });
//     create_sensor_list(objects.sensor_list, array_ofsensor_DATA);
// }

void setup() {
    Serial.begin(115200);
    static esp_lcd_panel_handle_t panel_handle = NULL;
    static esp_lcd_touch_handle_t tp_handle = NULL;

    tp_handle = touch_gt911_init();  
    panel_handle = waveshare_esp32_s3_rgb_lcd_init(); 
    wavesahre_rgb_lcd_bl_on();   
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));

    ESP_LOGI(TAG, "Display LVGL demos");

    if (lvgl_port_lock(-1)) {
        ui_init();
        setup_ui();
        lvgl_port_unlock();
    }
    wifi_init();

    if(!LittleFS.begin(true)){
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    File file = LittleFS.open("/sensor_config.toml");
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    
    Serial.println("File Content:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
    // esp_task_wdt_config_t wdt_config = {
    //     .timeout_ms = 5000,
    //     .trigger_panic = true,
    // };

    // esp_task_wdt_init(&wdt_config);
    // esp_task_wdt_add(xTaskGetCurrentTaskHandle());
}
void loop() {
    // LVGL events handle interactions
    // doWork();
    // esp_task_wdt_reset();
}

