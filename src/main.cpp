// src/main.cpp
#include <Arduino.h>
#include "core/Application.h"

Application* app = nullptr;

void setup() {
    app = new Application();
    app->initialize();
}

void loop() {
    if (app) {
        app->update();
    }
}
