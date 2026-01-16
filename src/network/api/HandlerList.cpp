//
// Created by launa on 15/01/2026.
//

#include "HandlerList.h"
#include <ArduinoJson.h>

#include "utils/StringUtils.h"
#include "services/output/LedService.h"
#include "services/sensors/SensorManager.h"
#include "services/sensors/SensorService.h"

HandlerList::HandlerList() {
    // Enregistrement des routes API

    addHandler(
        Route(
            "/help",
            HTTP_GET,
            [this](WebServer& server) {
                StaticJsonDocument<2048> doc;
                JsonArray routesArray = doc.createNestedArray("routes");

                for (const Route& route : handlers) {
                    JsonObject routeObj = routesArray.createNestedObject();
                    routeObj["path"] = route.getPath();
                    routeObj["method"] = route.getMethodString();
                    routeObj["description"] = route.getDescription();
                }

                String json;
                serializeJson(doc, json);
                server.send(200, "application/json", json);
            },
            "Liste toutes les routes disponibles avec leurs methodes et descriptions"
        )
    );

    addHandler(
        Route(
            "/led/on",
            HTTP_POST,
            [](WebServer& server) {
                    LedService::getInstance()->ledOn();
                    server.send(200, "text/plain", "Led allumee");
                },
                "Allume la led"
            )
    );

    addHandler(
        Route(
            "/led/off",
            HTTP_POST,
            [](WebServer& server) {
                    LedService::getInstance()->ledOff();
                    server.send(200, "text/plain", "Led eteinte");
                },
                "Eteint la led"
            )
    );

    addHandler(
    Route(
        "/led/toggle",
        HTTP_POST,
        [](WebServer& server) {
            LedService *ledService = LedService::getInstance();

            ledService->changeStatus();
            const int status = ledService->getStatus();

            StaticJsonDocument<128> doc;
            doc["status"] = status;

            String json;
            serializeJson(doc, json);
            server.send(200, "application/json", json);
            },
            "Change l'allumage de la led"
        )
);

    addHandler(
        Route(
            "/led",
            HTTP_GET,
            [](WebServer& server) {
                    const LedService *ledService = LedService::getInstance();
                    const int status = ledService->getStatus();
                    const int pos = ledService->getLedPos();

                    StaticJsonDocument<128> doc;
                    doc["status"] = status;
                    doc["pos"] = pos;

                    String json;
                    serializeJson(doc, json);
                    server.send(200, "application/json", json);
                },
                "Renvoie les informations de la led"
            )
    );
    addHandler(
        Route(
            "/led/threshold",
            HTTP_POST,
            [](WebServer& server) {
                    if (!server.hasArg("plain")) {
                        server.send(400, "text/plain", "Missing JSON body");
                        return;
                    }

                    StaticJsonDocument<256> doc;
                    const DeserializationError err = deserializeJson(doc, server.arg("plain"));

                    if (err) {
                        server.send(400, "text/plain", "Invalid JSON");
                        return;
                    }

                    int threshold_sensor = doc["sensor"];
                    int threshold_val = doc["val"];
                    int threshold_mode = doc["mode"];
                    LedService *ledService = LedService::getInstance();

                    ledService->setSensorTreshold(threshold_sensor, threshold_val, threshold_mode);
                    server.send(200, "text/plain", "Threshold modifie");
                },
                "Change le threshold"
            )
    );

    addHandler(
        Route(
            "/led/threshold",
            HTTP_DELETE,
            [](WebServer& server) {
                    LedService::getInstance()->deleteThreshold();
                    server.send(200, "text/plain", "Threshold supprime");
                },
                "Supprime le threshold"
            )
    );

    addHandler(
        Route(
            "/led/threshold",
            HTTP_GET,
            [](WebServer& server) {
                    const LedService *ledService = LedService::getInstance();
                    const int sensor = ledService->getThresholdSensor();
                    const int val = ledService->getThresholdValue();
                    const int mode = ledService->getThresholdMode();

                    StaticJsonDocument<256> doc;
                    doc["sensor"] = sensor;
                    doc["val"] = val;
                    doc["mode"] = mode;


                    String json;
                    serializeJson(doc, json);
                    server.send(200, "application/json", json);
                },
                "Recupere les informations du threshold"
            )
    );

    addHandler(
        Route(
            "/sensor",
            HTTP_GET,
            [](WebServer& server) {

                    if (server.hasArg("id")) {
                        const int id = server.arg("id").toInt();
                        handleSingleSensor(server, id);
                        return;
                    }

                    if (server.hasArg("ids")) {
                        const String ids = server.arg("ids");
                        handleMultipleSensors(server, ids);
                        return;
                    }

                    handleAllSensors(server);
                },
                "Recupere les informations du/des sensors"
            )
    );

    addHandler(
        Route(
            "/status",
            HTTP_GET,
            [](WebServer& server) {
                StaticJsonDocument<128> doc;

                if (server.hasArg("led")) {
                        const JsonArray ledsArray = doc.createNestedArray("leds");

                        const int led = server.arg("led").toInt();
                        if (led == 1) {
                            const LedService *ledService = LedService::getInstance();
                            const JsonObject ledObj = ledsArray.createNestedObject();
                            const int status = ledService->getStatus();
                            const int pos = ledService->getLedPos();

                            ledObj["status"] = status;
                            ledObj["pos"] = pos;

                        }

                    }

                if (server.hasArg("sensor_ids")) {
                        const JsonArray sensorsArray = doc.createNestedArray("sensors");
                        const String ids = server.arg("sensor_ids");
                        const std::vector<int> sensorIds = StringUtils::splitIds(ids);
                        const std::vector<SensorService *> sensors = SensorManager::getInstance()->getSensorsByIds(sensorIds);
                        for (SensorService *sensorService : sensors) {
                            if (!sensorService) continue;
                            JsonObject sensorObj = sensorsArray.createNestedObject();
                            sensorObj["id"] = sensorService->getId();
                            sensorObj["name"] = sensorService->getName();
                            sensorObj["val"] = sensorService->readSensor();
                            sensorObj["unit"] = sensorService->getUnit();
                            sensorObj["pos"] = sensorService->getSensorPos();
                        }
                    }

                if (server.hasArg("threshold")) {
                        const JsonArray thresholdsArray = doc.createNestedArray("thresholds");
                        const int threshold = server.arg("threshold").toInt();
                        if (threshold == 1) {
                            const JsonObject thresholdObj = thresholdsArray.createNestedObject();
                            const LedService *ledService = LedService::getInstance();
                            thresholdObj["sensor"] = ledService->getThresholdSensor();
                            thresholdObj["val"] = ledService->getThresholdValue();
                            thresholdObj["mode"] = ledService->getThresholdMode();
                        }
                    }
                String json;
                serializeJson(doc, json);
                server.send(200, "application/json", json);
                },
                "Recupere des informations combinees (led, sensors, threshold)"
            )
    );
}

// Gere la requete pour un seul capteur

// Gere la requete pour un seul capteur
void HandlerList::handleSingleSensor(WebServer& server, const int sensor) {
    auto sensorManager = SensorManager::getInstance();
    if (!sensorManager) {
        server.send(500, "application/json", R"({"error":"SensorManager not initialized"})");
        return;
    }

    SensorService *sensorService = sensorManager->getSensorById(sensor);
    if (!sensorService) {
        server.send(404, "application/json", R"({"error":"Sensor not found"})");
        return;
    }

    StaticJsonDocument<512> doc;
    JsonArray sensorsArray = doc.createNestedArray("sensors");

    JsonObject sensorObj = sensorsArray.createNestedObject();
    sensorObj["id"] = sensorService->getId();
    sensorObj["name"] = sensorService->getName();
    sensorObj["val"] = sensorService->readSensor();
    sensorObj["unit"] = sensorService->getUnit();
    sensorObj["pos"] = sensorService->getSensorPos();

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// Gere la requete pour tous les capteurs
void HandlerList::handleAllSensors(WebServer& server) {
    const auto sensorManager = SensorManager::getInstance();
    if (!sensorManager) {
        server.send(500, "application/json", R"({"error":"SensorManager not initialized"})");
        return;
    }

    const std::vector<SensorService *> sensors = sensorManager->getAllSensors();

    StaticJsonDocument<1024> doc;
    const JsonArray sensorsArray = doc.createNestedArray("sensors");

    for (SensorService *sensorService : sensors) {
        if (!sensorService) continue;

        JsonObject sensorObj = sensorsArray.createNestedObject();
        sensorObj["id"] = sensorService->getId();
        sensorObj["name"] = sensorService->getName();
        sensorObj["val"] = sensorService->readSensor();
        sensorObj["unit"] = sensorService->getUnit();
        sensorObj["pos"] = sensorService->getSensorPos();
    }

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// Gere la requete pour plusieurs capteurs specifiques
void HandlerList::handleMultipleSensors(WebServer& server, const String& ids) {
    const std::vector<int> sensorIds = StringUtils::splitIds(ids);

    const auto sensorManager = SensorManager::getInstance();
    if (!sensorManager) {
        server.send(500, "application/json", R"({"error":"SensorManager not initialized"})");
        return;
    }

    const std::vector<SensorService *> sensors = sensorManager->getSensorsByIds(sensorIds);

    StaticJsonDocument<1024> doc;
    const JsonArray sensorsArray = doc.createNestedArray("sensors");

    for (SensorService *sensorService : sensors) {
        if (!sensorService) continue;

        JsonObject sensorObj = sensorsArray.createNestedObject();
        sensorObj["id"] = sensorService->getId();
        sensorObj["name"] = sensorService->getName();
        sensorObj["val"] = sensorService->readSensor();
        sensorObj["unit"] = sensorService->getUnit();
        sensorObj["pos"] = sensorService->getSensorPos();
    }

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// Ajoute une route a la liste des handlers
void HandlerList::addHandler(const Route& handler) {
    handlers.push_back(handler);
}

// Retourne la liste de tous les handlers
std::vector<Route> HandlerList::getHandlers() {
    return handlers;
}
