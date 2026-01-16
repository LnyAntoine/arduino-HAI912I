//
// Created by launa on 15/01/2026.
//

#ifndef UNTITLED_HANDLERLIST_H
#define UNTITLED_HANDLERLIST_H
#include "Route.h"


class HandlerList {
private:
    std::vector<Route> handlers;
public:
    HandlerList();

    static void handleSingleSensor(WebServer &server, int sensor);

    std::vector<Route> getHandlers();
    int getHandlersSize();
    void addHandler(const Route& handler);

    static void handleMultipleSensors(WebServer &server, String ids);

    static void handleAllSensors(WebServer &server);
};


#endif //UNTITLED_HANDLERLIST_H