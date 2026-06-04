#include "data_communication.h"

struct_message incomingReadings;
struct_message outgoingSetpoints;
struct_pairing pairingData;

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

// callback when data is sent
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status)
{
    char macStr[18];
    Serial.print("Last Packet Send Status: ");
    Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success to " : "Delivery Fail to ");

    const uint8_t *mac_addr = info->des_addr;
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print(macStr);
    Serial.println();
}

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len)
{
    const uint8_t *mac_addr = recv_info->src_addr; // ← get MAC this way now
    if (len <= 0)
    {
        Serial.println("Received empty packet");
        return;
    }

    Serial.print(len);
    Serial.println(" bytes of new data received.");

    uint8_t type = incomingData[0];

    switch (type)
    {
    case DATA:
        Serial.println("Data message received, but data handling is not implemented yet.");
        break;

    case PAIRING:
    {
        memcpy(&pairingData, incomingData, sizeof(pairingData));

        Serial.println(pairingData.msgType);
        Serial.println(pairingData.id);

        Serial.print("Pairing request from MAC Address: ");
        Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                      pairingData.macAddr[0],
                      pairingData.macAddr[1],
                      pairingData.macAddr[2],
                      pairingData.macAddr[3],
                      pairingData.macAddr[4],
                      pairingData.macAddr[5]);

        Serial.print(" on channel ");
        Serial.println(pairingData.channel);

        memcpy(clientMacAddress, pairingData.macAddr, 6);

        if (pairingData.id > 0 &&
            pairingData.msgType == PAIRING)
        {
            pairingData.id = 0; // server ID

            WiFi.softAPmacAddress(pairingData.macAddr);

            Serial.print("Pairing MAC Address: ");
            Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                          clientMacAddress[0],
                          clientMacAddress[1],
                          clientMacAddress[2],
                          clientMacAddress[3],
                          clientMacAddress[4],
                          clientMacAddress[5]);

            pairingData.channel = chan;

            Serial.println("Sending pairing response");

            esp_now_send(
                clientMacAddress,
                reinterpret_cast<uint8_t *>(&pairingData),
                sizeof(pairingData));

            addPeer(clientMacAddress);
        }
        break;
    }

    default:
        Serial.printf("Unknown packet type: %u\n", type);
        break;
    }
}

void initESP_NOW()
{
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

    // Add broadcast address so we can receive pairing requests from any channel
    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    memset(&slave, 0, sizeof(slave));
    memcpy(slave.peer_addr, broadcastAddress, 6);
    slave.channel = 0; // 0 = current channel
    slave.encrypt = false;
    esp_now_add_peer(&slave);
}