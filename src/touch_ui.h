#pragma once
#include <lvgl.h>
#include "rgb_lcd_port.h"
#include "gt911.h"
#include "lvgl_port.h"
// #include "../lvgl.h"
#include "ui.h"
#include "screens.h"
#include <demos/lv_demos.h>
#include <iostream>
#include <list>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <vector>
#include "ui.h"
#include <string>
#define MAX_SENSORS 50



void setup_main_ui();
void setup_ui();
void refresh_data();
// void textarea_event_handler(lv_event_t * e);

// void btnm_event_handler(lv_event_t * e);

// void create_numpad(lv_obj_t * parent);
// void create_sensor_list(lv_obj_t * parent, std::string sensor_name);