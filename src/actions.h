
#include <Arduino.h>
#include"rf_switch.h"

#define OUTPUT_SERIAL 1

#ifndef __ACTIONS_H__
#define __ACTIONS_H__

class Action {
    public:
        virtual void performAction() = 0;
        virtual String getResponse() = 0;
        virtual ~Action()  {};
};

class SwitchAction : public Action {
    public:
        SwitchAction(RFSwitch* sw) : Action() {
            rfSwitch = sw;
        }
        void setOn(bool onOff) {
            on = onOff;
        }
        void performAction();
        String getResponse();

    private:
        RFSwitch* rfSwitch;
        bool on;
};

class InvalidAction : public Action {
    public:
        InvalidAction() : Action() {}

        void performAction();
        String getResponse();
};

#endif