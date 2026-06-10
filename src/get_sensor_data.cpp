#include "get_sensor_data.h"

String sensors_list[MAX_SENSORS];
int sensor_count = 0;

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

    // ── Rebuild sensors_list from the now-resident doc ────────────────────
    // (keeps get_sensor_list() working exactly as before, just no file I/O)
    sensor_count = 0;

    for (JsonObject s : g_cfg["sensors_two_point"].as<JsonArray>())
        if (sensor_count < MAX_SENSORS)
            sensors_list[sensor_count++] = s["id"].as<String>();

    for (JsonObject s : g_cfg["sensors_height"].as<JsonArray>())
        if (sensor_count < MAX_SENSORS)
            sensors_list[sensor_count++] = s["id"].as<String>();
}

void save_sensor_config() // only touches flash when YOU decide to
{
    if (!g_cfg_loaded)
        return;
    File file = LittleFS.open("/sensor_config.json", "w");
    if (!file)
        return;
    serializeJson(g_cfg, file);
    file.close();
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

// ── Shared array-search helper (avoids duplication) ──────────────────────
static JsonObject find_sensor(const String &id)
{
    for (JsonObject s : g_cfg["sensors_two_point"].as<JsonArray>())
        if (s["id"].as<String>() == id)
            return s;

    for (JsonObject s : g_cfg["sensors_height"].as<JsonArray>())
        if (s["id"].as<String>() == id)
            return s;

    return JsonObject(); // null object — callers check with .isNull()
}

// ── Pure RAM reads ────────────────────────────────────────────────────────
float get_sensor_value(const String &id, SensorField field)
{
    if (!g_cfg_loaded)
        return NAN;

    JsonObject s = find_sensor(id);
    if (s.isNull())
        return NAN;

    switch (field)
    {
    case CURRENT:
        return s["current"] | NAN;
    case SENSOR_TYPE:
        return s["sensor_type"] | NAN;
    case CURRENT_VOL:
        return s["currnt_vol"] | NAN;
    case AVG_VOL:
        return s["avg_vol"] | NAN;
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
    return NAN;
}

// ── Pure RAM writes (flash only when save_sensor_config() is called) ──────
void write_sensor_value(const String &id, SensorField field, float value)
{
    if (!g_cfg_loaded)
        return;

    JsonObject s = find_sensor(id);
    if (s.isNull())
        return;

    switch (field)
    {
    case CURRENT:
        s["current"] = value;
        break;
    case CURRENT_VOL:
        s["currnt_vol"] = value;
        break;
    case AVG_VOL:
        s["avg_vol"] = value;
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
    // No file write here — call save_sensor_config() when you're ready to persist
}

// ── Utility ───────────────────────────────────────────────────────────────
std::string fmt_float(float v, int precision)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%.*f", precision, v);
    return std::string(buf);
}
