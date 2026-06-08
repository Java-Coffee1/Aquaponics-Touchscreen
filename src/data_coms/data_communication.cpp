#include "data_communication.h"

struct_message incomingMessage;
struct_message outgoingMessage;

String success;

float incoming_board_id;
float incoming_reading;
float incoming_request_type;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    if (status == 0)
    {
        success = "Delivery Success :)";
    }
    else
    {
        success = "Delivery Fail :(";
    }
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));
    Serial.print("Bytes received: ");
    Serial.println(len);
    incoming_board_id = incomingMessage.board_id;
    incoming_reading = incomingMessage.reading;
    Serial.print("Board ID: ");
    Serial.println(incoming_board_id);
    Serial.print("Type: ");
    Serial.println(incomingMessage.type);
    Serial.print("Reading: ");
    Serial.println(incoming_reading);
    Serial.print("Request Type: ");
    Serial.println(incoming_request_type);
}

void outgoing_message(float board_id, String type, float reading, float request_type)
{
    outgoingMessage.board_id = board_id;
    outgoingMessage.type = type;
    outgoingMessage.reading = reading;
    outgoingMessage.request_type = request_type;

    esp_err_t result = esp_now_send(serverAddress, (uint8_t *)&outgoingMessage, sizeof(outgoingMessage));
    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }
}

void refresh_sensor_data(String name)
{
    Serial.println(name);

    // Example usage: send a message with board_id=2, sensor_id=3, reading=6.7, request_type=0 (data)
    outgoing_message(2, name, 6.7, 0);
}