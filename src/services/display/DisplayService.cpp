#include "DisplayService.h"
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <WiFi.h>

#include "config/config.h"
#include "../output/LedService.h"
#include "../sensors/SensorManager.h"
#include "../sensors/SensorService.h"


DisplayService::DisplayService() : tft(TFT_eSPI()) {}

DisplayService* DisplayService::getInstance() {
    static DisplayService instance;
    return &instance;
}

void DisplayService::begin() {
    tft.init();
    tft.setRotation(1);  // Mode paysage
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Allumer le backlight
}

// Affiche le statut de connexion WiFi
void DisplayService::showWiFiStatus(const String& ip) {
    clear();
    tft.setCursor(10, 50);
    tft.println("WiFi Connecte");
    tft.setCursor(10, 80);
    tft.println("IP: " + ip);
}

// Affiche les informations de seuil
void DisplayService::showThresholdInfo() {
    clear();
    tft.setCursor(10, 5);
    const int sensorId = LedService::getInstance()->getThresholdSensor();
    if (sensorId != -1) {
        const SensorService* sensor = SensorManager::getInstance()->getSensorById(sensorId);
        tft.printf("Capteurs de seuil : \n %s", sensor!=nullptr? sensor->getName().c_str() : "Aucun");
    } else {
        tft.printf("Capteurs de seuil : \n Aucun");
    }

    tft.setCursor(10, 60);
    tft.printf("Valeur de seuil: \n %d", LedService::getInstance()->getThresholdValue());
    tft.setCursor(10, 100);
    const int thresholdMode = LedService::getInstance()->getThresholdMode();
    String mode;
    switch (thresholdMode) {
        case 0:
            mode = "Superieur";
            break;
        case 1:
            mode = "Inferieur";
            break;
        case 2:
            mode = "Switch";
            break;
        default :
            mode = "Aucun";
            break;
    }
    tft.printf("Mode de seuil: \n %s", mode.c_str());
}

// Affiche les donnees d'un capteur specifique
void DisplayService::showSensorData(const int sensorId) {
    const SensorManager* sensorManager = SensorManager::getInstance();
    if (!sensorManager) {
        clear();
        tft.setCursor(10, 60);
        tft.printf("SensorManager null");
        return;
    }

    SensorService *sensor = sensorManager->getSensorById(sensorId);
    if (!sensor) {
        clear();
        tft.setCursor(10, 60);
        tft.printf("Capteur %d introuvable", sensorId);
        return;
    }

    clear();
    tft.setCursor(10, 70);
    tft.printf("%s: \n %f %s", sensor->getName().c_str(), sensor->getValue(), sensor->getUnit().c_str());
}

void DisplayService::clear() {
    tft.fillScreen(TFT_BLACK);
}

void DisplayService::showLedStatus(const bool isOn) {
    clear();
    tft.setCursor(10, 100);
    tft.setTextColor(isOn ? TFT_GREEN : TFT_RED, TFT_BLACK);
    tft.printf("LED: %s", isOn ? "ON" : "OFF");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

// Affiche le menu correspondant a l'index selectionne
void DisplayService::show(const int selectedIndex) {
    switch (selectedIndex) {
        case 0:
            showWiFiStatus(WiFi.localIP().toString());
            break;
        case 1:
            showSensorData(0);
            break;
        case 2 :
            showSensorData(1);
            break;
        case 3 :
            showLedStatus(LedService::getInstance()->getStatus());
            break;

        case 4 :
            showThresholdInfo();
            break;
        default:
            clear();
            tft.setCursor(10, 10);
            tft.println("Menu Inconnu");
            break;
    }
}

void DisplayService::update() {
    // Cette méthode est appelée périodiquement pour rafraîchir l'affichage
    // Elle peut être utilisée pour des animations, des mises à jour d'état, etc.
    // Pour l'instant, elle ne fait rien de spécifique car les mises à jour
    // sont déjà gérées par les méthodes show* appelées explicitement
}

