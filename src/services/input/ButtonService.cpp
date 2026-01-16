#include "ButtonService.h"


ButtonService::ButtonService() 
    : lastDebounceLeft(0), lastDebounceRight(0),
      lastStateLeft(HIGH), lastStateRight(HIGH),
      pressStartLeft(0), pressStartRight(0) {}

ButtonService* ButtonService::getInstance() {
    static ButtonService instance;
    return &instance;
}

void ButtonService::begin() {
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
}

ButtonEvent ButtonService::checkButtons() {
    const unsigned long now = millis();
    
    // Vérifier bouton gauche
    const bool currentLeft = digitalRead(BUTTON_LEFT);
    if (currentLeft != lastStateLeft && (now - lastDebounceLeft) > DEBOUNCE_DELAY) {
        lastDebounceLeft = now;
        lastStateLeft = currentLeft;
        
        if (currentLeft == LOW) {
            pressStartLeft = now;
        } else if (pressStartLeft > 0) {
            const unsigned long pressDuration = now - pressStartLeft;
            pressStartLeft = 0;
            
            if (pressDuration >= LONG_PRESS_DURATION) {
                return BUTTON_LEFT_LONG_PRESS;
            }
            return BUTTON_LEFT_PRESS;
        }
    }
    
    // Vérifier bouton droit
    const bool currentRight = digitalRead(BUTTON_RIGHT);
    if (currentRight != lastStateRight && (now - lastDebounceRight) > DEBOUNCE_DELAY) {
        lastDebounceRight = now;
        lastStateRight = currentRight;

        if (currentRight == LOW) {
            pressStartRight = now;
        } else if (pressStartRight > 0) {
            const unsigned long pressDuration = now - pressStartRight;
            pressStartRight = 0;

            if (pressDuration >= LONG_PRESS_DURATION) {
                return BUTTON_RIGHT_LONG_PRESS;
            }
            return BUTTON_RIGHT_PRESS;
        }
    }

    return BUTTON_NONE;
}
