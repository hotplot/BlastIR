#include "wifi.h"
#include "mqtt.h"
#include "credentials.h"

#include <ESP8266WiFi.h>

namespace
{
    WiFiEventHandler connectHandler, disconnectHandler;

    void gotIPHandler(const WiFiEventStationModeGotIP &event)
    {
        wifi::connected = true;
        Serial.print("WiFi connected with IP ");
        Serial.println(event.ip);
        mqtt::connect();
    }

    void disconnectedHandler(const WiFiEventStationModeDisconnected &event)
    {
        wifi::connected = false;
        Serial.println("WiFi disconnected");
    }
}

namespace wifi
{
    bool connected = false;

    void setup()
    {
        WiFi.persistent(false);

        connectHandler = WiFi.onStationModeGotIP(&gotIPHandler);
        disconnectHandler = WiFi.onStationModeDisconnected(&disconnectedHandler);

        WiFi.mode(WIFI_AP_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
}
