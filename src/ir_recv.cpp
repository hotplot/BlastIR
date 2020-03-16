#include "ir.h"
#include "mqtt.h"

#include <Arduino.h>
#include <Ticker.h>

#define IR_INPUT_PIN            (0)
#define MAX_RECORDED_DURATIONS  (1024)

namespace
{
    uint16_t numRecordedDurations = 0;
    uint16_t recordedDurations[MAX_RECORDED_DURATIONS];
    unsigned long lastTransitionTime = 0;

    Ticker recordingCompletionTimer;

    void ICACHE_RAM_ATTR receiverPinInterrupt()
    {
        unsigned long currentTime = micros();
        unsigned long elapsedTime = currentTime - lastTransitionTime;
        lastTransitionTime = currentTime;

        // Cancel the existing completion timer
        recordingCompletionTimer.detach();        

        if (elapsedTime > 10000)
        {
            // If the duration was too high, assume this is the beginning of a new transmission and ignore the data
        } 
        else if (numRecordedDurations < MAX_RECORDED_DURATIONS)
        {
            // Otherwise record the duration and schedule the completion timer for 10ms
            ir::hasRecordedData = true;
            recordedDurations[numRecordedDurations++] = elapsedTime;
            recordingCompletionTimer.once_ms(10, ir::stopRecording);
        }
        else
        {
            // If we have run out of buffer space, set a flag and give up
            ir::recordingOverflowed = true;
            ir::stopRecording();
        }
    }
}

namespace ir
{
    bool isRecording = false;
    bool hasRecordedData = false;
    bool recordingOverflowed = false;

    void setupReceive()
    {
        pinMode(IR_INPUT_PIN, INPUT);
    }

    void startRecording()
    {
        isRecording = true;
        hasRecordedData = recordingOverflowed = false;

        lastTransitionTime = 0;
        numRecordedDurations = 0;
        
        attachInterrupt(IR_INPUT_PIN, receiverPinInterrupt, CHANGE);
    }
    
    void stopRecording()
    {
        recordingCompletionTimer.detach();        
        detachInterrupt(IR_INPUT_PIN);
        ir::isRecording = false;
    }

    void processRecordedData()
    {
        String output = "";

        for (int index = 0; index < numRecordedDurations; ++index)
        {
            output += recordedDurations[index];
            output += " ";
        }

        mqtt::publishReceivedIRData(output.c_str());
        
        if (recordingOverflowed)
            mqtt::publishLog("Received external IR command but it was too large for the receive buffer");
        else
            mqtt::publishLog("Received external IR command");

        numRecordedDurations = 0;
        hasRecordedData = false;
    }
}
