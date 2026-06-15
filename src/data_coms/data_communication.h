#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#include "sensor_setup.h"
#include "get_sensor_data.h"

#define SENSOR_NAME_MAX_LEN 32

extern uint8_t serverAddress[];

enum RequestType : uint8_t
{
    DATA,
    REQUEST,
    SENSOR_LIST
};

typedef struct struct_message
{
    float board_id;
    char type[SENSOR_NAME_MAX_LEN];
    float reading;
    RequestType request_type; // 0 for data, 1 for request
} struct_message;

extern esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void outgoing_message(float board_id, String type, float reading, RequestType request_type);
void api_get_sensor_data(String name);
