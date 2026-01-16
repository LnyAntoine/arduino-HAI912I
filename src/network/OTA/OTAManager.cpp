//
// Created by launa on 16/01/2026.
//

#include "OTAManager.h"

// Configure le mot de passe OTA
void OTAManager::setPassword(const char* password) {
    _password = password;

}

// Configure le port OTA
void OTAManager::setPort(uint16_t port) {
    _port = port;
}

// Active ou desactive les logs de debug
void OTAManager::setDebug(bool enable) {
    _debug = enable;
}

OTAManager::OTAManager(const char* hostname)
    : _hostname(hostname),
      _password(nullptr),
      _port(3232),
      _debug(true),
      _initialized(false) {
}

bool OTAManager::begin() {
    if (WiFi.status() != WL_CONNECTED) {
        if (_debug) {
            Serial.println("[OTA] Erreur: WiFi non connecte!");
        }
        return false;
    }

    ArduinoOTA.setHostname(_hostname);
    ArduinoOTA.setPort(_port);

    if (_password != nullptr && strlen(_password) > 0) {
        ArduinoOTA.setPassword(_password);
        if (_debug) {
            Serial.println("[OTA] Mot de passe active");
        }
    }

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {
            type = "filesystem";
        }
        Serial.println("[OTA] Debut de la mise a jour: " + type);
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("[OTA] Mise a jour terminee!");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        unsigned int percent = (progress / (total / 100));
        Serial.printf("[OTA] Progression: %u%%\r", percent);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("[OTA] Erreur[%u]: ", error);

        if (error == OTA_AUTH_ERROR) {
            Serial.println("Echec d'authentification");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Echec du demarrage");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Echec de connexion");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Echec de reception");
        } else if (error == OTA_END_ERROR) {
            Serial.println("Echec de finalisation");
        }
    });

    ArduinoOTA.begin();
    _initialized = true;

    if (_debug) {
        Serial.println("OTA initialise avec succes");
        Serial.printf("  Hostname: %s\n", _hostname);
        Serial.printf("  IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("  Port: %d\n", _port);
        Serial.println("  Pret pour les mises a jour OTA!");
    }

    return true;
}

void OTAManager::handle() {
    if (_initialized) {
        ArduinoOTA.handle();
    }
}