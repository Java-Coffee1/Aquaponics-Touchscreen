#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#include "sensor_setup.h"
#include "get_sensor_data.h"

extern uint8_t serverAddress[];

typedef struct struct_message
{
    float board_id;
    float sensor_id;
    float reading;
} struct_message;

extern esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void outgoing_message(float board_id, float sensor_id, float reading);
