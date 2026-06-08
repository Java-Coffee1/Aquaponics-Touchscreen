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
#include "get_sensor_data.h"
#include "data_coms/data_communication.h"

void setup_main_ui();
void setup_ui();
void two_point_calibration_refresh();
void sensor_hight_calibration_refresh();
// void textarea_event_handler(lv_event_t * e);

// void btnm_event_handler(lv_event_t * e);

// void create_numpad(lv_obj_t * parent);
// void create_sensor_list(lv_obj_t * parent, std::string sensor_name);