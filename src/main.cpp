#include <Arduino.h>

#include "ir.h"
#include "mqtt.h"
#include "wifi.h"

void setup()
{
    Serial.begin(115200);
    Serial.println();

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

    // If the IR recorder has finished receiving a command, process it
    if (ir::hasRecordedData && ir::isRecording == false) {
        ir::processRecordedData();
    }
    
    // If the IR recorder is stopped, restart it
    if (ir::isRecording == false) {
        ir::startRecording();
    }
}
