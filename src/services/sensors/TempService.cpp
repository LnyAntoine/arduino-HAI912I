//
// Created by launa on 15/01/2026.
//

#include "TempService.h"
#include "../../config/config.h"
#include <Arduino.h>
TempService *TempService::getInstance() {
    static TempService instance;
    return &instance;
}

TempService::TempService() {
    id = 1;
    tempPosition = TEMP_PIN;
    value = 0;
    oldValue = 0;
    unit = "Celsius";
    name = "Temperature Sensor";
}

float TempService::readSensor() {
    oldValue = value;

    int thermRaw = analogRead(getSensorPos()); //Lire le sensor sur getSensorPos

    if (thermRaw <= 0) {
        return value;
    }

    float thermResistance = TEMP_RES / ((4095.0f / thermRaw) - 1.0f);

    // Calcul direct de la température (Steinhart-Hart simplifié / B-parameter)
    float lnRratio = log(thermResistance / THERMISTORNOMINAL);
    float invT = (lnRratio / BCOEFFICIENT) + (1.0f / (TEMPERATURENOMINAL + 273.15f));

    if (invT == 0.0f) {
        return value;
    }

    float tempKelvin = 1.0f / invT;
    float tempCelsius = tempKelvin - 273.15f;

    value = tempCelsius;
    return value;
}

int TempService::getSensorPos() const {
    return tempPosition;
}
int TempService::getId() const{
    return id;
}
float TempService::getOldValue() const {
    return oldValue;
}
std::string TempService::getUnit() const {
    return unit;
}
std::string TempService::getName() const {
    return name;
}
