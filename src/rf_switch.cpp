#include <Arduino.h>
#include "rf_switch.h"

RFSwitch::RFSwitch(RCSwitch *sender, const char* on, const char* off) {
    this->sender = sender;
    this->onMessage = on;
    this->offMessage = off;
}

void RFSwitch::turnOn() {
    Serial.println("turning on");
    repeatSend(onMessage);
}

void RFSwitch::turnOff() {
    Serial.println("turning off");
    repeatSend(offMessage);
}

void RFSwitch::repeatSend(const char* message) {
    Serial.println(message);
    for (int i = 0; i < 3; i++) {
        sender->sendTriState(message);
        Serial.print(".");
        delay(10);
    }
    Serial.println();
}