#pragma once
#include <stdint.h>
#include <demos/lv_demos.h>
#include <iostream>
#include <vector>
#include <list>
#include <Arduino.h>

#define MAX_SENSORS 50
#define BOARD_ID 1

extern String get_sensor_name;

// ---------------- Sensor Field Enum ----------------
enum SensorType : uint8_t
{
    TWOPOINT,
    OFFSET_HIGHT
};

enum SensorField : uint8_t
{
    CURRENT,
    SENSOR_TYPE,
    CURRENT_VOL,
    AVG_VOL,
    MIN,
    MAX,
    LAST_MIN,
    LAST_MAX,
    HEIGHT_OFFSET,
    LAST_HEIGHT_OFFSET,
    SENSOR_FIELD_MAX
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