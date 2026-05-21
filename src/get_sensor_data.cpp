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

void write_sensor_value(const String &id, SensorField field, float value)
{
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file) return;
    StaticJsonDocument<4096> doc;
    DeserializationError err = deserializeJson(doc, file);
    file.close();
    if (err) return;
    JsonArray arr1 = doc["sensors_two_point"];
    JsonArray arr2 = doc["sensors_height"];

    auto updateArray = [&](JsonArray arr) -> bool {
        for (JsonObject s : arr) {
            if (s["id"].as<String>() == id) {

                switch (field) {
                    case CURRENT:
                        s["current"] = value;
                        break;
                    case AVG:
                        s["avg"] = value;
                        break;

                    case MIN:
                        s["min"] = value;
                        break;

                    case MAX:
                        s["max"] = value;
                        break;

                    case LAST_MIN:
                        s["last_min"] = value;
                        break;

                    case LAST_MAX:
                        s["last_max"] = value;
                        break;

                    case HEIGHT_OFFSET:
                        s["height_offset"] = value;
                        break;

                    case LAST_HEIGHT_OFFSET:
                        s["last_height_offset"] = value;
                        break;
                }
                return true;
            }
        }
        return false;
    };
    if (!updateArray(arr1)) {
        updateArray(arr2);
    }

    // Write back to file
    file = LittleFS.open("/sensor_config.json", "w");
    if (!file) return;
    serializeJson(doc, file);
    file.close();
}