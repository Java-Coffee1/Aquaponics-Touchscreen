#include "sensor_setup.h"
#include "get_sensor_data.h"
#include "esp_now.h"
#include "WiFi.h"
#include <Arduino.h>
typedef struct struct_pairing
{ // new structure for pairing
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t channel;
} struct_pairing;

extern esp_now_peer_info_t slave;
extern int chan;

void initESP_NOW();