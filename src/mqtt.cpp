#include "mqtt.h"
#include "wifi.h"
#include "ir.h"
#include "credentials.h"

#include <AsyncMqttClient.h>
#include <Ticker.h>

namespace
{
    AsyncMqttClient mqttClient;
    Ticker reconnectTimer;

    void connectHandler(bool persistentSession)
    {
        mqtt::connected = true;
        Serial.println("MQTT connected");
        mqttClient.subscribe(MQTT_IR_SEND_TOPIC, 2);
    }

    void disconnectHandler(AsyncMqttClientDisconnectReason reason)
    {
        mqtt::connected = false;
        Serial.print("MQTT disconnected with reason ");
        Serial.println((int8_t)reason);
        reconnectTimer.once(2, mqtt::connect);
    }

    void messageHandler(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
    {
        if (payload != NULL)
            ir::processCommand(payload);
    }
}

namespace mqtt
{
    bool connected = false;

    void setup()
    {
        mqttClient.setServer(MQTT_HOST, MQTT_PORT);
        mqttClient.setCredentials(MQTT_USER, MQTT_PASS);

        mqttClient.onConnect(connectHandler);
        mqttClient.onDisconnect(disconnectHandler);
        mqttClient.onMessage(messageHandler);
    }

    void connect()
    {
        if (wifi::connected)
            mqttClient.connect();
    }

    void publishReceivedIRData(const char *message)
    {
        if (mqtt::connected)
            mqttClient.publish(MQTT_IR_RECV_TOPIC, 0, 0, message);
    }

    void publishLog(const char *message)
    {
        if (mqtt::connected)
            mqttClient.publish(MQTT_IR_LOG_TOPIC, 0, 0, message);
    }
}
