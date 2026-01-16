//
// Created by launa on 15/01/2026.
//

#ifndef UNTITLED_DISPLAYSERVICE_H
#define UNTITLED_DISPLAYSERVICE_H
#ifndef DISPLAYSERVICE_H
#define DISPLAYSERVICE_H

#include <TFT_eSPI.h>
#include <Arduino.h>

class DisplayService {
private:
    TFT_eSPI tft;

    DisplayService();  // Constructeur privé pour singleton

public:
    static DisplayService* getInstance();

    void begin();
    void clear();
    void showWiFiStatus(const String& ip);

    void showThresholdInfo();

    void showSensorData(int sensorId);
    void showLedStatus(bool isOn);
    void show(int selectedIndex);

    static void update();  // Appelée dans loop() pour rafraîchir l'affichage
};

#endif

#endif //UNTITLED_DISPLAYSERVICE_H