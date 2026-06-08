#include "get_sensor_data.h"

String sensors_list[MAX_SENSORS];
SensorInformation sensors_info[MAX_SENSORS];

int sensor_count = 0;
bool sensors_dirty = false;

// -----------------------------
// Load ALL sensors into RAM
// -----------------------------
void load_sensors_to_ram()
{
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file)
        return;

    StaticJsonDocument<4096> doc;
    if (deserializeJson(doc, file))
        return;
    file.close();

    sensor_count = 0;

    auto loadArray = [&](JsonArray arr)
    {
        for (JsonObject s : arr)
        {
            if (sensor_count >= MAX_SENSORS)
                return;

            SensorInformation &sensor = sensors_info[sensor_count++];

            sensor.id = s["id"] | "";
            sensor.sensor_type = s["sensor_type"] | 0;

            sensor.current = s["current"] | 0;
            sensor.currnt_vol = s["currnt_vol"] | 0;
            sensor.avg_vol = s["avg_vol"] | 0;

            sensor.min = s["min"] | 0;
            sensor.max = s["max"] | 0;

            sensor.last_min = s["last_min"] | 0;
            sensor.last_max = s["last_max"] | 0;

            sensor.height_offset = s["height_offset"] | 0;
            sensor.last_height_offset = s["last_height_offset"] | 0;
        }
    };

    loadArray(doc["sensors_two_point"]);
    loadArray(doc["sensors_height"]);
}

// -----------------------------
// Fast RAM lookup
// -----------------------------
SensorInformation *find_sensor(const String &id)
{
    for (int i = 0; i < sensor_count; i++)
    {
        if (sensors_info[i].id == id)
            return &sensors_info[i];
    }
    return nullptr;
}

// -----------------------------
// Read from RAM
// -----------------------------
float get_sensor_value(const String &id, SensorField field)
{
    SensorInformation *s = find_sensor(id);
    if (!s)
        return NAN;

    switch (field)
    {
    case CURRENT:
        return s->current;
    case SENSOR_TYPE:
        return s->sensor_type;
    case CURRENT_VOL:
        return s->currnt_vol;
    case AVG_VOL:
        return s->avg_vol;
    case MIN:
        return s->min;
    case MAX:
        return s->max;
    case LAST_MIN:
        return s->last_min;
    case LAST_MAX:
        return s->last_max;
    case HEIGHT_OFFSET:
        return s->height_offset;
    case LAST_HEIGHT_OFFSET:
        return s->last_height_offset;
    }

    return NAN;
}

// -----------------------------
// Write to RAM + JSON sync
// -----------------------------
void write_sensor_value(const String &id, SensorField field, float value)
{
    SensorInformation *s = find_sensor(id);
    if (!s)
        return;

    switch (field)
    {
    case CURRENT:
        s->current = value;
        break;
    case CURRENT_VOL:
        s->currnt_vol = value;
        break;
    case AVG_VOL:
        s->avg_vol = value;
        break;
    case MIN:
        s->min = value;
        break;
    case MAX:
        s->max = value;
        break;
    case LAST_MIN:
        s->last_min = value;
        break;
    case LAST_MAX:
        s->last_max = value;
        break;
    case HEIGHT_OFFSET:
        s->height_offset = value;
        break;
    case LAST_HEIGHT_OFFSET:
        s->last_height_offset = value;
        break;
    default:
        return;
    }
    sensors_dirty = true;
}
// -----------------------------
// Add sensor (RAM + file reload)
// -----------------------------
void add_sensor(const String &id, const String &type)
{
    File file = LittleFS.open("/sensor_config.json", "r");
    if (!file)
        return;

    StaticJsonDocument<4096> doc;
    if (deserializeJson(doc, file))
        return;
    file.close();

    JsonArray targetArray;

    if (type == "two_point")
        targetArray = doc["sensors_two_point"];
    else if (type == "height")
        targetArray = doc["sensors_height"];
    else
        return;

    JsonObject sensor = targetArray.createNestedObject();

    sensor["id"] = id;
    sensor["sensor_type"] = 0;

    sensor["current"] = 0;
    sensor["currnt_vol"] = 0;
    sensor["avg_vol"] = 0;

    sensor["min"] = 0;
    sensor["max"] = 0;

    sensor["last_min"] = 0;
    sensor["last_max"] = 0;

    sensor["height_offset"] = 0;
    sensor["last_height_offset"] = 0;

    file = LittleFS.open("/sensor_config.json", "w");
    if (!file)
        return;

    serializeJsonPretty(doc, file);
    file.close();

    // refresh RAM after change
    load_sensors_to_ram();
}

void save_all_sensors_to_file()
{
    File file = LittleFS.open("/sensor_config.json", "w");
    if (!file)
        return;

    StaticJsonDocument<4096> doc;

    JsonArray arr1 = doc.createNestedArray("sensors_two_point");
    JsonArray arr2 = doc.createNestedArray("sensors_height");

    for (int i = 0; i < sensor_count; i++)
    {
        SensorInformation &s = sensors_info[i];

        JsonObject obj;

        if (s.sensor_type == 0)
        {
            obj = arr1.createNestedObject();
        }
        else
        {
            obj = arr2.createNestedObject();
        }

        obj["id"] = s.id;
        obj["sensor_type"] = s.sensor_type;

        obj["current"] = s.current;
        obj["currnt_vol"] = s.currnt_vol;
        obj["avg_vol"] = s.avg_vol;

        obj["min"] = s.min;
        obj["max"] = s.max;

        obj["last_min"] = s.last_min;
        obj["last_max"] = s.last_max;

        obj["height_offset"] = s.height_offset;
        obj["last_height_offset"] = s.last_height_offset;
    }

    serializeJsonPretty(doc, file);
    file.close();
}