
#ifndef MQTT_h
#define MQTT_h

namespace mqtt
{
    extern bool connected;

    void setup();
    void connect();

    void publishReceivedIRData(const char *message);
    void publishLog(const char *message);
}

#endif
