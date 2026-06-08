#include "data_communication.h"

struct_message incomingMessage;
struct_message outgoingMessage;

String success;

float incoming_board_id;
float incoming_sensor_id;
float incoming_reading;

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
    incoming_sensor_id = incomingMessage.sensor_id;
    incoming_reading = incomingMessage.reading;
    Serial.print("Board ID: ");
    Serial.println(incoming_board_id);
    Serial.print("Sensor ID: ");
    Serial.println(incoming_sensor_id);
    Serial.print("Reading: ");
    Serial.println(incoming_reading);
}

void outgoing_message(float board_id, float sensor_id, float reading)
{
    outgoingMessage.board_id = board_id;
    outgoingMessage.sensor_id = sensor_id;
    outgoingMessage.reading = reading;

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