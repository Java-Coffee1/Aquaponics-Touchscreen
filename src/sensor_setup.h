#pragma once
#include <stdint.h>
#include <demos/lv_demos.h>
#include <iostream>
#include <vector>
#include <list>
#include <Arduino.h>

#define MAX_SENSORS 50

// ---------------- Sensor Field Enum ----------------

enum SensorField : uint8_t
{
    CURRENT = 0,
    SENSOR_TYPE = 1,
    CURRENT_VOL = 2,
    AVG_VOL = 3,
    MIN = 4,
    MAX = 5,
    LAST_MIN = 6,
    LAST_MAX = 7,
    HEIGHT_OFFSET = 8,
    LAST_HEIGHT_OFFSET = 9,
    UNKNOWN = 255
};

// ---------------- Sensor Data Struct ----------------
struct SensorInformation
{
    String board_id;
    String id;
    String type;
    float current;
    float sensor_type;
    float currnt_vol;
    float avg_vol;
    float min;
    float max;
    float last_min;
    float last_max;
    float height_offset;
    float last_height_offset;
};