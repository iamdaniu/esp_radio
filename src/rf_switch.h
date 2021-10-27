#ifndef __RF_SWITCH__
#define __RF_SWITCH__

#include <RCSwitch.h>

#define RESEND_COUNT 10

class RFSwitch {
    public:
        RFSwitch(RCSwitch *sender, const char* onMessage, const char* offMessage);
    
        void turnOn();
        void turnOff();
    
    private:
        RCSwitch *sender;
        const char* onMessage;
        const char* offMessage;

        void repeatSend(const char* message);
};

#endif