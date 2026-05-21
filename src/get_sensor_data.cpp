#include "get_sensor_data.h"

String sensors_list[MAX_SENSORS];
int sensor_count = 0;


void get_sensor_list() {
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file) return;

    StaticJsonDocument<4096> doc;
    DeserializationError err = deserializeJson(doc, file);
    file.close();

    if (err) return;

    sensor_count = 0;

    // -------- sensors_two_point --------
    JsonArray arr1 = doc["sensors_two_point"];
    for (JsonObject s : arr1) {
        if (sensor_count < MAX_SENSORS) {
            sensors_list[sensor_count++] = s["id"].as<String>();
        }
    }

    // -------- sensors_height --------
    JsonArray arr2 = doc["sensors_height"];
    for (JsonObject s : arr2) {
        if (sensor_count < MAX_SENSORS) {
            sensors_list[sensor_count++] = s["id"].as<String>();
        }
    }
}


SensorInformation sensors_info[MAX_SENSORS];

float get_sensor_value(const String &id, SensorField field)
{
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file) return NAN;
    StaticJsonDocument<4096> doc;
    DeserializationError err = deserializeJson(doc, file);
    file.close();
    if (err) return NAN;
    JsonArray arr1 = doc["sensors_two_point"];
    JsonArray arr2 = doc["sensors_height"];

    auto searchArray = [&](JsonArray arr) -> float {
        for (JsonObject s : arr) {
            if (s["id"].as<String>() == id) {

                switch (field) {
                    case CURRENT:
                        return s["current"] | NAN;
                    case AVG:
                        return s["avg"] | NAN;

                    case MIN:
                        return s["min"] | NAN;

                    case MAX:
                        return s["max"] | NAN;

                    case LAST_MIN:
                        return s["last_min"] | NAN;

                    case LAST_MAX:
                        return s["last_max"] | NAN;

                    case HEIGHT_OFFSET:
                        return s["height_offset"] | NAN;

                    case LAST_HEIGHT_OFFSET:
                        return s["last_height_offset"] | NAN;
                }
            }
        }
        return NAN;
    };
    float result = searchArray(arr1);
    if (!isnan(result)) return result;
    return searchArray(arr2);
}