#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "config.h"
#include "OTAManager.h"
#include "api/APIServer.h"
#include "services/ButtonService.h"
#include "services/DisplayService.h"
#include "services/LedService.h"
#include "services/sensors/LightService.h"
#include "services/sensors/SensorManager.h"
#include "wifiConfig.h"


APIServer *ApiServer;
SensorManager *SensorManager;
LedService *LedService;
DisplayService *display;
ButtonService *buttons;
OTAManager ota("TTGO-Meteo");
void setup() {



    Serial.begin(115200);
    delay(1000);
    analogSetAttenuation(ADC_11db);
    Serial.println("\n\nDémarrage...");

    // Initialiser l'écran en premier
    display = DisplayService::getInstance();
    display->begin();

    // Initialiser les boutons
    buttons = ButtonService::getInstance();
    buttons->begin();

    // Initialiser le SensorManager
    auto sensorManager = ::SensorManager::getInstance();
    if (!sensorManager) {
        Serial.println("Erreur: SensorManager non initialisé");
    } else {
        Serial.printf("SensorManager initialisé avec %d capteurs\n", sensorManager->getAllSensors().size());
    }

    // Afficher message de démarrage
    display->showWiFiStatus("Connexion...");


    // Connexion au WiFi
    Serial.print("Connexion au WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnecté !");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    if (ota.begin()) {
        Serial.println("OTA prêt !");
    } else {
        Serial.println("Échec OTA");
    }

    display->showWiFiStatus(WiFi.localIP().toString());
    delay(2000);  // Laisser le temps de voir l'IP

    // Afficher les données du premier capteur
    display->showSensorData(0);


    HandlerList* handlers = new HandlerList();
    ApiServer = new APIServer(80, handlers);
    ApiServer->begin();

    Serial.println("\nServer API Connecté\n");
}
unsigned long previousLedUpdate = 0;   // temps de la dernière mise à jour
constexpr unsigned long LED_UPDATE_INTERVAL = 500; // ms, ici toutes les 500ms
const int nb_menu = 4;
int menuIndex = 0;
void loop() {
    ApiServer->handleClient();
    ota.handle();

    // Gérer les boutons
    if (buttons) {
        const ButtonEvent event = buttons->checkButtons();

        switch (event) {
            case BUTTON_LEFT_PRESS:
                menuIndex--;
                if (menuIndex < 0) menuIndex = nb_menu;
                break;
            case BUTTON_RIGHT_PRESS:
                menuIndex++;
                if (menuIndex > nb_menu) menuIndex = 0;
                break;
            default: ;
        }
        // Mise à jour périodique de l'affichage
        if (display) {
            static unsigned long lastDisplayUpdate = 0;
            if (millis() - lastDisplayUpdate >= 1000) {
                lastDisplayUpdate = millis();
                if (!display) return;
                display->show(menuIndex);
            }
        }

    }



    unsigned long currentMillis = millis();
    if (currentMillis - previousLedUpdate >= LED_UPDATE_INTERVAL) {
        previousLedUpdate = currentMillis;

        auto sensorManager = SensorManager::getInstance();
        if (!sensorManager) return;

        auto ledService = LedService::getInstance();
        if (!ledService) return;

        int thresholdSensor = ledService->getThresholdSensor();

        if (thresholdSensor != -1) {
            SensorService *sensor = sensorManager->getSensorById(thresholdSensor);
            if (sensor) {
                ledService->updateLedThreshold(
                    sensor->readSensor(),
                    sensor->getOldValue()
                );
            }
        }
    }
}
