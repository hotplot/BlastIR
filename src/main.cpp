#include <Arduino.h>

#include "ir.h"
#include "mqtt.h"
#include "wifi.h"

void setup()
{
    Serial.begin(115200);

    // Setup IR receiver and transmitter
    ir::setupSend();
    ir::setupReceive();

    // Setup MQTT callbacks etc
    mqtt::setup();

    // Establish a WiFi connection
    // A WiFi connection callback is responsible for establishing a MQTT connection once we have an IP
    wifi::setup();
}

void loop()
{
    delay(500);

    if (ir::hasRecordedData) {
        ir::processRecordedData();
    } else if (ir::isRecording == false) {
        ir::startRecording();
    }
}
