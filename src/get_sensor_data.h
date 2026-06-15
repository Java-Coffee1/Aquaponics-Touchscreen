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
struct Sensor
{
    String id;
    String sensor_type;

    float current = 0;

    int current_vol = 0;
    int avg_vol = 0;
    int max = 0;
    int min = 0;
    int last_max = 0;
    int last_min = 0;

    int height_offset = 0;
    int last_height_offset = 0;
};
extern std::vector<Sensor> sensors;
extern String sensors_list[MAX_SENSORS];
extern int sensor_count;
extern std::vector<Sensor> sensors;

// ---------------- Function Prototypes ----------------
void get_sensor_list();
float get_sensor_value(const String &id, SensorField field);
void write_sensor_value(const String &id, SensorField field, float value);
void load_sensor_config(); // call once in setup()
void save_sensor_config(); // call when you want to persist to flash

std::string fmt_float(float v, int precision = 2);