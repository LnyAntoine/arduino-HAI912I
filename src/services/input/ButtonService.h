//
// Created by launa on 15/01/2026.
//


#ifndef BUTTONSERVICE_H
#define BUTTONSERVICE_H

#include <Arduino.h>
#include "config/config.h"

enum ButtonEvent {
    BUTTON_NONE,
    BUTTON_LEFT_PRESS,
    BUTTON_RIGHT_PRESS,
    BUTTON_LEFT_LONG_PRESS,
    BUTTON_RIGHT_LONG_PRESS
};

class ButtonService {
private:

    unsigned long lastDebounceLeft;
    unsigned long lastDebounceRight;
    bool lastStateLeft;
    bool lastStateRight;
    unsigned long pressStartLeft;
    unsigned long pressStartRight;

    static const unsigned long DEBOUNCE_DELAY = 50;
    static const unsigned long LONG_PRESS_DURATION = 1000;

    ButtonService();

public:
    static ButtonService* getInstance();

    static void begin();
    ButtonEvent checkButtons();
};

#endif


