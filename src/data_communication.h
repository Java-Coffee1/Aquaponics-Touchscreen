#pragma once
#include "sensor_setup.h"
#include "get_sensor_data.h"
#include "esp_now.h"
#include "WiFi.h"
#include <Arduino.h>

typedef struct struct_message
{
    uint8_t msgType;
    uint8_t id;
    float temp;
    float hum;
    unsigned int readingId;
} struct_message;

typedef struct struct_pairing
{
    uint8_t msgType;
    uint8_t id;
    uint8_t macAddr[6];
    uint8_t channel;
} struct_pairing;

extern struct_message incomingReadings;
extern struct_message outgoingSetpoints;
extern struct_pairing pairingData;

enum MessageType
{
    PAIRING,
    DATA
};

extern MessageType messageType;

extern esp_now_peer_info_t slave;
extern int chan;
extern uint8_t clientMacAddress[];

void initESP_NOW();
bool addPeer(const uint8_t *peer_addr);

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status);
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len);