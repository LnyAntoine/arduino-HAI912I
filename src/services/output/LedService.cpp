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

void LedService::setSensorTreshold(int sensorNumber, int tresholdVal,int mode) {
    threshold = tresholdVal;
    sensor_threshold_nb = sensorNumber;
    threshold_mode = mode;
}
void LedService::deleteThreshold() {
    threshold = -1;
    sensor_threshold_nb = -1;
    threshold_mode = -1;
}
void LedService::updateLedThreshold(int value,int oldValue) {
    if (threshold == -1) return;
    if (threshold_mode == -1) return;
    if (threshold_mode == 0) {
        //GreaterThan
        if (value < threshold) {
            if (getStatus() == 1) {ledOff();}
        }
        else if (value > threshold) {
            if (getStatus() == 0) {ledOn();}
        }
    }
    else if (threshold_mode == 1) {
        //LesserThan
        if (value > threshold) {
            if (getStatus() == 1) {ledOff();}
        }
        else if (value < threshold) {
            if (getStatus() == 0) {ledOn();}
        }
    }
    else if (threshold_mode == 2) {
        //Switch mode
        if ((value > threshold && oldValue < threshold)||(
            value < threshold && oldValue > threshold)) {
            changeStatus();
        }

    }

}

void LedService::ledOff() {
    digitalWrite(getLedPos(), LOW);
    led_status = 0;
}

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


void LedService::changeStatus() {
    if (getStatus() == 0) {ledOn();} else {ledOff();}
}

int LedService::getStatus() const {
    return led_status;
}

LedService::LedService() : sensor_threshold_nb(-1), threshold(-1), led_status(0), threshold_mode(-1) {
    ledPosition = LED_PIN;
}
