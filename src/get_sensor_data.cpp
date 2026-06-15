#include "get_sensor_data.h"

String sensors_list[MAX_SENSORS];
int sensor_count = 0;
std::vector<Sensor> sensors;

// ── Single in-RAM copy of the config ──────────────────────────────────────
static StaticJsonDocument<4096> g_cfg; // lives in RAM for the lifetime of the program
static bool g_cfg_loaded = false;

void load_sensor_config()
{
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file)
        return;

    DeserializationError err = deserializeJson(g_cfg, file);
    file.close();
    if (err)
        return;

    g_cfg_loaded = true;

    for (JsonObject sensorJson : g_cfg["sensors_two_point"].as<JsonArray>())
    {
        Sensor sensor;

        sensor.id = sensorJson["id"].as<String>();
        sensor.sensor_type = sensorJson["sensor_type"].as<String>();
        sensor.current = sensorJson["current"] | 0;

        sensor.current_vol = sensorJson["currnt_vol"] | 0;
        sensor.avg_vol = sensorJson["avg_vol"] | 0;
        sensor.max = sensorJson["max"] | 0;
        sensor.min = sensorJson["min"] | 0;

        sensor.height_offset = sensorJson["height_offset"] | 0;

        sensors.push_back(sensor);
    }
}
float get_sensor_value(const String &id, SensorField field)
{
    for (Sensor &s : sensors)
    {
        if (s.id != id)
            continue;

        switch (field)
        {
        case CURRENT:
            return s.current;
        case CURRENT_VOL:
            return s.current_vol;
        case AVG_VOL:
            return s.avg_vol;
        case MIN:
            return s.min;
        case MAX:
            return s.max;
        case LAST_MIN:
            return s.last_min;
        case LAST_MAX:
            return s.last_max;
        case HEIGHT_OFFSET:
            return s.height_offset;
        case LAST_HEIGHT_OFFSET:
            return s.last_height_offset;
        default:
            return 0;
        }
    }
    return 0;
}

void write_sensor_value(const String &id, SensorField field, float value)
{
    for (Sensor &s : sensors)
    {
        if (s.id != id)
            continue;

        switch (field)
        {
        case CURRENT:
            s.current = value;
            break;
        case CURRENT_VOL:
            s.current_vol = (int)value;
            break;
        case AVG_VOL:
            s.avg_vol = (int)value;
            break;
        case MIN:
            s.min = (int)value;
            break;
        case MAX:
            s.max = (int)value;
            break;
        case LAST_MIN:
            s.last_min = (int)value;
            break;
        case LAST_MAX:
            s.last_max = (int)value;
            break;
        case HEIGHT_OFFSET:
            s.height_offset = (int)value;
            break;
        case LAST_HEIGHT_OFFSET:
            s.last_height_offset = (int)value;
            break;
        default:
            break;
        }
        return;
    }
}
// ── get_sensor_list() is now a no-op rebuild from RAM ─────────────────────
void get_sensor_list() // signature unchanged — callers unaffected
{
    if (!g_cfg_loaded)
        load_sensor_config(); // lazy-load safety net

    sensor_count = 0;

    for (JsonObject s : g_cfg["sensors_two_point"].as<JsonArray>())
        if (sensor_count < MAX_SENSORS)
            sensors_list[sensor_count++] = s["id"].as<String>();

    for (JsonObject s : g_cfg["sensors_height"].as<JsonArray>())
        if (sensor_count < MAX_SENSORS)
            sensors_list[sensor_count++] = s["id"].as<String>();
}

void save_sensor_config()
{
    if (!g_cfg_loaded)
        return;

    // overwrite the in-RAM json with current sensor values
    JsonArray arr = g_cfg["sensors_two_point"].as<JsonArray>();
    for (JsonObject obj : arr)
    {
        String id = obj["id"].as<String>();
        for (Sensor &s : sensors)
        {
            if (s.id != id)
                continue;

            obj["sensor_type"] = s.sensor_type;
            obj["current"] = s.current;
            obj["current_vol"] = s.current_vol;
            obj["avg_vol"] = s.avg_vol;
            obj["min"] = s.min;
            obj["max"] = s.max;
            obj["last_min"] = s.last_min;
            obj["last_max"] = s.last_max;
            obj["height_offset"] = s.height_offset;
            obj["last_height_offset"] = s.last_height_offset;
            break;
        }
    }

    // write updated json to flash
    File file = LittleFS.open("/sensor_config.json", "w");
    if (!file)
        return;
    serializeJson(g_cfg, file);
    file.close();
}

// ── Utility ───────────────────────────────────────────────────────────────
std::string fmt_float(float v, int precision)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%.*f", precision, v);
    return std::string(buf);
}
