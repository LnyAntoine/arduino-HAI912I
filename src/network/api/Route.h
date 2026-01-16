#ifndef ROUTE_H
#define ROUTE_H

#include <Arduino.h>
#include <functional>
#include <WebServer.h>

/**
 * @brief Type de fonction handler pour une route
 * Prend le serveur en paramètre pour accéder à request/response
 */
typedef std::function<void(WebServer&)> RouteHandler;

/**
 * @brief Classe représentant une route API
 *
 * Encapsule le chemin, la méthode HTTP et le handler
 */
class Route {
private:
    String path;
    HTTPMethod method;
    RouteHandler handler;
    String description;

public:
    /**
     * @brief Constructeur
     * @param path Chemin de la route (ex: "/api/led")
     * @param method Méthode HTTP (HTTP_GET, HTTP_POST, etc.)
     * @param handler Fonction à appeler pour cette route
     * @param description Description de la route (optionnel)
     */
    Route(const String& path,
          HTTPMethod method,
          const RouteHandler &handler,
          const String& description = "");

    /**
     * @brief Récupère le chemin de la route
     */
    String getPath() const;

    /**
     * @brief Récupère la méthode HTTP
     */
    HTTPMethod getMethod() const;

    /**
     * @brief Récupère le handler
     */
    RouteHandler getHandler() const;

    /**
     * @brief Récupère la description
     */
    String getDescription() const;

    /**
     * @brief Récupère la méthode HTTP sous forme de string
     */
    String getMethodString() const;
};

#endif // ROUTE_H