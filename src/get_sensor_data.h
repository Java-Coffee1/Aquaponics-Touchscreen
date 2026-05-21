#pragma once
#include <demos/lv_demos.h>
#include <iostream>
#include <list>
#include "LittleFS.h"
#include <ArduinoJson.h>
#include <vector>
#include "ui.h"
#include <string>

#define MAX_SENSORS 50


// ---------------- Sensor Field Enum ----------------
enum SensorField {
    CURRENT,
    AVG,
    MIN,
    MAX,
    LAST_MIN,
    LAST_MAX,
    HEIGHT_OFFSET,
    LAST_HEIGHT_OFFSET
};

// ---------------- Sensor Data Struct ----------------
struct SensorInformation {
    String id;
    String type;

    float current;
    float avg;
    float min;
    float max;
    float last_min;
    float last_max;
    float height_offset;
    float last_height_offset;
};

// ---------------- External Globals ----------------
// (defined in .cpp)
extern String sensors_list[MAX_SENSORS];
extern int sensor_count;

extern SensorInformation sensors_info[MAX_SENSORS];

// ---------------- Function Prototypes ----------------
void get_sensor_list();
float get_sensor_value(const String &id, SensorField field);
void write_sensor_value(const String &id, SensorField field, float value);