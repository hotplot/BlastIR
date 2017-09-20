#include <Arduino.h>

#include "ir.h"
#include "mqtt.h"
#include "wifi.h"

#define HB_LED_PIN (2)

static void heartbeat()
{
    // Turn on the LED
    digitalWrite(HB_LED_PIN, LOW);

    // Short flashes indicate we are running fine
    // Long flashes indicate an error
    if (wifi::connected && mqtt::connected)
        delay(250);
    else
        delay(1000);
    
    // Turn off the LED
    digitalWrite(HB_LED_PIN, HIGH);
}

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

    // Setup heartbeat LED pin
    pinMode(HB_LED_PIN, OUTPUT);
    digitalWrite(HB_LED_PIN, HIGH);
}

void loop()
{
    unsigned long startTime = millis();
    heartbeat();
    unsigned long duration = millis() - startTime;
    delay(2000 - duration);

    if (ir::hasRecordedData) {
        ir::processRecordedData();
    } else if (ir::isRecording == false) {
        ir::startRecording();
    }
}
