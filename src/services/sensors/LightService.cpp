//
// Created by launa on 15/01/2026.
//

#include "LightService.h"
#include "../../config/config.h"
#include <Arduino.h>
LightService::LightService() {
    id = 0;
    lightPosition = LIGHT_PIN;
    value = 0;
    oldValue = 0;
    unit = "Lux";
    name = "Light Sensor";
}


LightService *LightService::getInstance() {
    static LightService instance;
    return &instance;
}

float LightService::readSensor() {
    oldValue = value;
    const int photoRaw  = analogRead(getSensorPos()); //Lire le sensor sur getSensorPos
    const float photoVoltage = photoRaw * (VOLTAGE / 4095.0);

    if (photoVoltage == 0.0f) {
        return value;
    }
    float photoResistance = LIGHT_RES * (VOLTAGE - photoVoltage) / photoVoltage;

    if (photoResistance == 0.0f) {
        return value;
    }

    float lux = 500000.0 / photoResistance;
    value = lux;
    return value;
}
int LightService::getSensorPos() const {
    return lightPosition;
}
std::string LightService::getUnit() const {
    return unit;
}
int LightService::getId() const {
    return id;
}
float LightService::getOldValue() const {
    return oldValue;
}
std::string LightService::getName() const {
    return name;
}
