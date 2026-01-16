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
    int thermRaw = analogRead(getSensorPos());

    printf("\n=== DEBUG THERMISTOR ===\n");
    printf("Raw: %d\n", thermRaw);

    // Étape 1 : Calcul résistance
    float thermResistance = TEMP_RES / ((4095.0 / thermRaw) - 1.0);
    printf("Résistance: %.2f Ω\n", thermResistance);

    // Étape 2 : R/Ro
    float step1 = thermResistance / THERMISTORNOMINAL;
    printf("R/Ro = %.6f\n", step1);

    // Étape 3 : ln(R/Ro)
    float step2 = log(step1);
    printf("ln(R/Ro) = %.6f\n", step2);

    // Étape 4 : / B
    float step3 = step2 / BCOEFFICIENT;
    printf("ln(R/Ro)/B = %.9f\n", step3);  // Plus de précision

    // Étape 5 : + 1/To - DEBUG DÉTAILLÉ
    printf("TEMPERATURENOMINAL = %.2f\n", TEMPERATURENOMINAL);
    float to_kelvin = TEMPERATURENOMINAL + 273.15;
    printf("To (Kelvin) = %.2f K\n", to_kelvin);
    float one_over_to = 1.0 / to_kelvin;
    printf("1/To = %.9f\n", one_over_to);

    float step4 = step3 + one_over_to;
    printf("step3 + 1/To = %.9f\n", step4);

    // Étape 6 : Inversion
    float step5 = 1.0 / step4;
    printf("1/x (Kelvin) = %.2f K\n", step5);

    // Étape 7 : Celsius
    float tempCelsius = step5 - 273.15;
    printf("Température finale: %.2f °C\n", tempCelsius);
    printf("========================\n\n");

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
