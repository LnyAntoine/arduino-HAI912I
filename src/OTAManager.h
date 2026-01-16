//
// Created by launa on 16/01/2026.
//

#ifndef UNTITLED_OTAMANAGER_H
#define UNTITLED_OTAMANAGER_H

#include <ArduinoOTA.h>
#include <WiFi.h>

/**
 * @brief Gestionnaire OTA (Over-The-Air) pour ESP32
 *
 * Cette classe encapsule toute la logique OTA
 * pour la rendre simple à utiliser
 */
class OTAManager {
public:

    /**
     * @brief Constructeur
     * @param hostname Nom de l'appareil sur le réseau
     *
     * Exemple : OTAManager ota("MonESP32");
     */
    OTAManager(const char* hostname = "ESP32-OTA");

    /**
     * @brief Initialiser l'OTA
     * @return true si succès, false sinon
     */
    bool begin();

    /**
     * @brief Gérer les requêtes OTA
     */
    void handle();


    /**
     * @brief Définir un mot de passe
     * @param password Mot de passe pour sécuriser
     */
    void setPassword(const char* password);

    /**
     * @brief Définir le port OTA
     * @param port Numéro de port (défaut: 3232)
     */
    void setPort(uint16_t port);


    /**
     * @brief Activer/désactiver les messages de debug
     * @param enable true pour activer
     */
    void setDebug(bool enable);

private:

    const char* _hostname;

    const char* _password;

    uint16_t _port;

    bool _debug;

    bool _initialized;

    static void onStartCallback();
    static void onEndCallback();
    static void onProgressCallback(unsigned int progress, unsigned int total);
    static void onErrorCallback(ota_error_t error);

};

#endif //UNTITLED_OTAMANAGER_H