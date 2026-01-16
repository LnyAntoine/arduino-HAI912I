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
    int photoRaw  = analogRead(getSensorPos()); //Lire le sensor sur getSensorPos
    printf("Raw Light value: %d\n", photoRaw);
    float photoVoltage = photoRaw * (VOLTAGE / 4095.0);
    float photoResistance = LIGHT_RES * (VOLTAGE - photoVoltage) / photoVoltage;
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
