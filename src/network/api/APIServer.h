#ifndef API_SERVER_H
#define API_SERVER_H

#include <WebServer.h>
#include <ArduinoJson.h>
#include <vector>

#include "HandlerList.h"
#include "Route.h"

/**
 * @brief Classe de gestion du serveur API REST modulaire
 */
class APIServer {
private:
    WebServer server;
    HandlerList* handlers;
    std::vector<Route> routes;  // Liste des routes
    bool ledState;

    // Fonctions utilitaires
    void sendJSON(int code, const String& json);
    void registerRoutes();


public:
    /**
     * @brief Constructeur
     * @param port Port du serveur (par défaut 80)
     * @param handlers la liste de handlers
     */
    APIServer(int port = 80,HandlerList* handlers=nullptr);

    /**
     * @brief Ajoute une route à la liste
     * @param route Route à ajouter
     */
    void addRoute(const Route& route);

    /**
     * @brief Initialise et démarre le serveur
     */
    void begin();

    /**
     * @brief Traite les requêtes
     */
    void handleClient();

    /**
     * @brief Affiche la liste des endpoints
     */
    void printEndpoints() const;

    /**
     * @brief Récupère la liste des routes
     */
    const std::vector<Route>& getRoutes();
};

#endif // API_SERVER_H