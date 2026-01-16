// src/core/Application.cpp
#include "Application.h"
#include "../config/wifiConfig.h"
#include <WiFi.h>

Application::Application()
    : display(nullptr)
    , buttons(nullptr)
    , leds(nullptr)
    , sensors(nullptr)
    , apiServer(nullptr)
    , ota(nullptr)
    , menuIndex(0)
    , nbMenu(4)
    , lastDisplayUpdate(0)
    , lastLedUpdate(0)
{
}

Application::~Application() {
    delete apiServer;
    delete ota;
}

void Application::initialize() {
    Serial.begin(115200);
    delay(1000);
    analogSetAttenuation(ADC_11db);
    Serial.println("\n\nDémarrage...");

    // Initialisation des services
    display = DisplayService::getInstance();
    display->begin();

    buttons = ButtonService::getInstance();
    buttons->begin();

    sensors = SensorManager::getInstance();
    if (!sensors) {
        Serial.println("Erreur: SensorManager non initialisé");
        return;
    }
    Serial.printf("SensorManager initialisé avec %d capteurs\n",
                  sensors->getAllSensors().size());

    leds = LedService::getInstance();
    if (!leds) {
        Serial.println("Erreur: LedService non initialisé");
        return;
    }

    // WiFi et réseau
    display->showWiFiStatus("Connexion...");
    initializeWiFi();

    // OTA
    ota = new OTAManager("TTGO-Meteo");
    if (ota->begin()) {
        Serial.println("OTA prêt !");
    }

    // API Server
    HandlerList* handlers = new HandlerList();
    apiServer = new APIServer(80, handlers);
    apiServer->begin();
    Serial.println("\nServer API Connecté\n");

    // Affichage initial
    display->showWiFiStatus(WiFi.localIP().toString());
    delay(2000);
    display->showSensorData(0);
}

void Application::initializeWiFi() {
    Serial.print("Connexion au WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnecté !");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());
}

void Application::update() {
    // Mise à jour des services réseau
    if (apiServer) apiServer->handleClient();
    if (ota) ota->handle();

    // Gestion des événements
    handleButtonEvents();
    updateDisplay();
    updateLeds();
}

void Application::handleButtonEvents() {
    if (!buttons) return;

    const ButtonEvent event = buttons->checkButtons();

    switch (event) {
        case BUTTON_LEFT_PRESS:
            menuIndex = (menuIndex - 1 + nbMenu + 1) % (nbMenu + 1);
            break;
        case BUTTON_RIGHT_PRESS:
            menuIndex = (menuIndex + 1) % (nbMenu + 1);
            break;
        default:
            break;
    }
}

void Application::updateDisplay() {
    if (!display) return;

    unsigned long currentMillis = millis();
    if (currentMillis - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
        lastDisplayUpdate = currentMillis;
        display->show(menuIndex);
    }
}

void Application::updateLeds() {
    if (!leds || !sensors) return;

    unsigned long currentMillis = millis();
    if (currentMillis - lastLedUpdate >= LED_UPDATE_INTERVAL) {
        lastLedUpdate = currentMillis;

        int thresholdSensor = leds->getThresholdSensor();
        if (thresholdSensor == -1) return;

        SensorService* sensor = sensors->getSensorById(thresholdSensor);
        if (sensor) {
            leds->updateLedThreshold(
                sensor->readSensor(),
                sensor->getOldValue()
            );
        }
    }
}
