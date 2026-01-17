// src/core/Application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>
#include "../services/display/DisplayService.h"
#include "../services/input/ButtonService.h"
#include "../services/output/LedService.h"
#include "../services/sensors/SensorManager.h"
#include "../network/api/APIServer.h"
#include "../network/OTA/OTAManager.h"

class Application {
private:
    // Services (injectés ou singletons)
    DisplayService* display;
    ButtonService* buttons;
    LedService* leds;
    SensorManager* sensors;
    APIServer* apiServer;
    OTAManager* ota;

    // État de l'application
    int menuIndex;
    const int nbMenu;
    unsigned long lastDisplayUpdate;
    unsigned long lastLedUpdate;
    unsigned long lastSensorUpdate;

    // Constantes
    static constexpr unsigned long DISPLAY_UPDATE_INTERVAL = 1000;
    static constexpr unsigned long LED_UPDATE_INTERVAL = 500;
    static constexpr unsigned long SENSOR_UPDATE_INTERVAL = 500;

    // Méthodes privées
    void initializeWiFi();
    void handleButtonEvents();
    void updateDisplay();
    void updateLeds();
    static void updateSensor(SensorService *sensor);
    void updateSensors();

public:
    Application();
    ~Application();

    void initialize();  // appelé dans setup()
    void update();      // appelé dans loop()
};

#endif // APPLICATION_H
