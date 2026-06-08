#pragma once
#include <demos/lv_demos.h>
#include <iostream>
#include <list>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <vector>
#include "ui.h"
#include "sensor_setup.h"
#define MAX_SENSORS 50

// ---------------- External Globals ----------------
// (defined in .cpp)
extern String sensors_list[MAX_SENSORS];
extern int sensor_count;

extern SensorInformation sensors_info[MAX_SENSORS];

// ---------------- Function Prototypes ----------------
void get_sensor_list();
float get_sensor_value(const String &id, SensorField field);
void write_sensor_value(const String &id, SensorField field, float value);
void load_sensors_to_ram();
void save_all_sensors_to_file();

std::string fmt_float(float v, int precision = 2);