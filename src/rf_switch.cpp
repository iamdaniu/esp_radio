#include <Arduino.h>
#include "rf_switch.h"
#include "actions.h"

RFSwitch::RFSwitch(RCSwitch *sender, const char* on, const char* off) {
    this->sender = sender;
    this->onMessage = on;
    this->offMessage = off;
}

void RFSwitch::turnOn() {
#if OUTPUT_SERIAL
    Serial.println("turning on");
#endif
    repeatSend(onMessage);
}

void RFSwitch::turnOff() {
#if OUTPUT_SERIAL
    Serial.println("turning off");
#endif
    repeatSend(offMessage);
}

void RFSwitch::repeatSend(const char* message) {
#if OUTPUT_SERIAL
    Serial.println(message);
#endif
    for (int i = 0; i < RESEND_COUNT; i++) {
        sender->sendTriState(message);
#if OUTPUT_SERIAL
        Serial.print(".");
#endif
        delay(10);
    }
#if OUTPUT_SERIAL
    Serial.println();
#endif
}