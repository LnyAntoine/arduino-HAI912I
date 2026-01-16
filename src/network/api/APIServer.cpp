#include "APIServer.h"

#include "HandlerList.h"
#include "config/config.h"

APIServer::APIServer(const int port, HandlerList* handlers) : server(port),handlers(handlers), ledState(false) {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void APIServer::addRoute(const Route& route) {
    routes.push_back(route);
}

void APIServer::registerRoutes() {
    for (const Route& route : handlers->getHandlers()) {
        addRoute(route);
    }
}

void APIServer::begin() {
    registerRoutes();

    // Configurer le serveur avec toutes les routes
    for (const Route& route : routes) {
        auto handler = route.getHandler();

        server.on(route.getPath(),
                  route.getMethod(),
                  [this, handler]() {
                      handler(this->server);
                  }
                  );
    }

    server.begin();
    Serial.println("✓ Serveur API démarré avec " + String(routes.size()) + " routes");
}

void APIServer::handleClient() {
    server.handleClient();
}

void APIServer::sendJSON(const int code, const String& json) {
    server.send(code, "application/json", json);
}

void APIServer::printEndpoints() const {
    Serial.println("\n=== Endpoints disponibles ===");
    for (const Route& route : routes) {
        Serial.printf("%-6s %s - %s\n",
                     route.getMethodString().c_str(),
                     route.getPath().c_str(),
                     route.getDescription().c_str());
    }
    Serial.println("=============================\n");
}

const std::vector<Route>& APIServer::getRoutes() {
    return routes;
}

