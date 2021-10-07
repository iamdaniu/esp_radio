
#include "actions.h"

void SwitchAction::performAction() {
    if (on) {
        rfSwitch->turnOn();
    } else {
        rfSwitch->turnOff();
    }
}

String SwitchAction::getResponse() {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    s += on ? "on" : "off";
    return s;
}

void InvalidAction::performAction() {
#if OUTPUT_SERIAL
    Serial.println("invalid request");
#endif
}

String InvalidAction::getResponse() {
    return "HTTP/1.1 404 \r\n";
}