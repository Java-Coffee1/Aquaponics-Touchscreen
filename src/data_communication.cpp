#include "data_communication.h"

bool addPeer(const uint8_t *peer_addr)
{ // add pairing
    memset(&slave, 0, sizeof(slave));
    const esp_now_peer_info_t *peer = &slave;
    memcpy(slave.peer_addr, peer_addr, 6);

    slave.channel = chan; // pick a channel
    slave.encrypt = 0;    // no encryption
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if (exists)
    {
        // Slave already paired.
        Serial.println("Already Paired");
        return true;
    }
    else
    {
        esp_err_t addStatus = esp_now_add_peer(peer);
        if (addStatus == ESP_OK)
        {
            // Pair success
            Serial.println("Pair success");
            return true;
        }
        else
        {
            Serial.println("Pair failed");
            return false;
        }
    }
}

void initESP_NOW()
{
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

// callback when data is sent
void OnDataSent(const wifi_tx_info_t *mac_addr, esp_now_send_status_t status)
{
    char macStr[18];
    Serial.print("Last Packet Send Status: ");
    Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success to " : "Delivery Fail to ");
    // Copies the receiver mac address to a string
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print(macStr);
    Serial.println();
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    Serial.print(len);
    Serial.println(" bytes of new data received.");
    StaticJsonDocument<1000> root;
    String payload;
    uint8_t type = incomingData[0]; // first message byte is the type of message
    switch (type)
    {
    case DATA: // the message is data type
        memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
        // create a JSON document with received data and send it by event to the web page
        root["id"] = incomingReadings.id;
        root["temperature"] = incomingReadings.temp;
        root["humidity"] = incomingReadings.hum;
        root["readingId"] = String(incomingReadings.readingId);
        serializeJson(root, payload);
        Serial.print("event send :");
        serializeJson(root, Serial);
        events.send(payload.c_str(), "new_readings", millis());
        Serial.println();
        break;

    case PAIRING: // the message is a pairing request
        memcpy(&pairingData, incomingData, sizeof(pairingData));
        Serial.println(pairingData.msgType);
        Serial.println(pairingData.id);
        Serial.print("Pairing request from MAC Address: ");
        printMAC(pairingData.macAddr);
        Serial.print(" on channel ");
        Serial.println(pairingData.channel);

        clientMacAddress[0] = pairingData.macAddr[0];
        clientMacAddress[1] = pairingData.macAddr[1];
        clientMacAddress[2] = pairingData.macAddr[2];
        clientMacAddress[3] = pairingData.macAddr[3];
        clientMacAddress[4] = pairingData.macAddr[4];
        clientMacAddress[5] = pairingData.macAddr[5];

        if (pairingData.id > 0)
        { // do not replay to server itself
            if (pairingData.msgType == PAIRING)
            {
                pairingData.id = 0; // 0 is server
                // Server is in AP_STA mode: peers need to send data to server soft AP MAC address
                WiFi.softAPmacAddress(pairingData.macAddr);
                Serial.print("Pairing MAC Address: ");
                printMAC(clientMacAddress);
                pairingData.channel = chan;
                Serial.println(" send response");
                esp_err_t result = esp_now_send(clientMacAddress, (uint8_t *)&pairingData, sizeof(pairingData));
                addPeer(clientMacAddress);
            }
        }
        break;
    }
}