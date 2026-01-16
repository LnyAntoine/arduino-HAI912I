//
// Created by launa on 15/01/2026.
//

#include "LedService.h"
#include "../../config/config.h"
#include <Arduino.h>

LedService *LedService::getInstance() {
    static LedService instance;
    return &instance;
}

int LedService::getLedPos() const {
    return ledPosition;
}

// Configure un seuil de declenchement pour la LED
void LedService::setSensorTreshold(int sensorNumber, int tresholdVal,int mode) {
    threshold = tresholdVal;
    sensor_threshold_nb = sensorNumber;
    threshold_mode = mode;
}

// Supprime le seuil configure
void LedService::deleteThreshold() {
    threshold = -1;
    sensor_threshold_nb = -1;
    threshold_mode = -1;
}

// Met a jour l'etat de la LED en fonction du seuil
void LedService::updateLedThreshold(int value,int oldValue) {
    if (threshold == -1) return;
    if (threshold_mode == -1) return;

    if (threshold_mode == 0) {
        // Mode superieur a
        if (value < threshold) {
            if (getStatus() == 1) {ledOff();}
        }
        else if (value > threshold) {
            if (getStatus() == 0) {ledOn();}
        }
    }
    else if (threshold_mode == 1) {
        // Mode inferieur a
        if (value > threshold) {
            if (getStatus() == 1) {ledOff();}
        }
        else if (value < threshold) {
            if (getStatus() == 0) {ledOn();}
        }
    }
    else if (threshold_mode == 2) {
        // Mode switch (bascule au passage du seuil)
        if ((value > threshold && oldValue < threshold)||(
            value < threshold && oldValue > threshold)) {
            changeStatus();
        }
    }

}

// Eteint la LED
void LedService::ledOff() {
    digitalWrite(getLedPos(), LOW);
    led_status = 0;
}

// Allume la LED
void LedService::ledOn() {
    digitalWrite(getLedPos(), HIGH);
    led_status = 1;
}

int LedService::getThresholdMode() const {
    return threshold_mode;
}

int LedService::getThresholdValue() const {
    return threshold;
}

int LedService::getThresholdSensor() const {
    return sensor_threshold_nb;
}

// Bascule l'etat de la LED
void LedService::changeStatus() {
    if (getStatus() == 0) {ledOn();} else {ledOff();}
}

int LedService::getStatus() const {
    return led_status;
}

LedService::LedService() : sensor_threshold_nb(-1), threshold(-1), led_status(0), threshold_mode(-1) {
    ledPosition = LED_PIN;
}
