
#ifndef IR_h
#define IR_h

namespace ir
{
    void setupSend();
    void setupReceive();
    
    extern bool isRecording;
    extern bool hasRecordedData;
    extern bool recordingOverflowed;

    void startRecording();
    void stopRecording();
    void processRecordedData();
    
    void clearCommandBuffer();
    void parseDurations(const char *payload);
    void transmit();
}

#endif
