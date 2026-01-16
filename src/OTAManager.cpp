//
// Created by launa on 16/01/2026.
//

#include "OTAManager.h"

void OTAManager::setPassword(const char* password) {
    _password = password;

}

void OTAManager::setPort(uint16_t port) {
    _port = port;
}

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
            Serial.println("[OTA] Erreur: WiFi non connecté !");
        }
        return false;
    }

    ArduinoOTA.setHostname(_hostname);

    ArduinoOTA.setPort(_port);

    if (_password != nullptr && strlen(_password) > 0) {
        ArduinoOTA.setPassword(_password);
        if (_debug) {
            Serial.println("[OTA] Mot de passe activé");
        }
    }
    ArduinoOTA.onStart([]() {

        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";      // Mise à jour du programme
        } else {
            type = "filesystem";  // Mise à jour du système de fichiers
        }
        Serial.println("\n[OTA] Début de la mise à jour: " + type);
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\n[OTA] Mise à jour terminée !");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

        unsigned int percent = (progress / (total / 100));
        // Calcule le pourcentage : (octets reçus / total) * 100

        Serial.printf("[OTA] Progression: %u%%\r", percent);
    });
    ArduinoOTA.onError([](ota_error_t error) {
        // ota_error_t = type d'erreur (enum)

        Serial.printf("\n[OTA] Erreur[%u]: ", error);

        if (error == OTA_AUTH_ERROR) {
            Serial.println("Échec d'authentification");
            // Mauvais mot de passe
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Échec du démarrage");
            // Impossible de commencer (pas assez de mémoire ?)
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Échec de connexion");
            // Problème réseau
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Échec de réception");
            // Données corrompues
        } else if (error == OTA_END_ERROR) {
            Serial.println("Échec de finalisation");
            // Impossible de finaliser la mise à jour
        }
    });

    ArduinoOTA.begin();
    // Lance le serveur OTA sur le port 3232
    // L'ESP32 écoute maintenant les demandes de mise à jour

    _initialized = true;

    if (_debug) {
        Serial.println("\n✓ OTA initialisé avec succès");
        Serial.printf("  Hostname: %s\n", _hostname);
        Serial.printf("  IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("  Port: %d\n", _port);
        Serial.println("  Prêt pour les mises à jour OTA !");
    }

    return true;  // ↑ Succès !
}

void OTAManager::handle() {
    if (_initialized) {
        ArduinoOTA.handle();
    }
}