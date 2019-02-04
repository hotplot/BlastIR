#include <stddef.h>

#ifndef IR_h
#define IR_h

namespace ir
{
    // Initialisation functions to be called at startup
    void setupSend();
    void setupReceive();
    
    // True if the IR command recorder is ready to receive a message,
    // False if a complete message has been received and is waiting to be processed.
    extern bool isRecording;

    // True if a partial or complete IR recording has been received.
    extern bool hasRecordedData;

    // True if an IR command was received but was too large to store.
    extern bool recordingOverflowed;

    // Functions for controlling the IR command recorder.
    void startRecording();
    void stopRecording();
    void processRecordedData();
    
    // Functions for controlling the IR command transmitter.
    void clearCommandBuffer();
    void parseDurations(const char *message, size_t len, bool messageComplete);
    void transmit();
}

#endif
