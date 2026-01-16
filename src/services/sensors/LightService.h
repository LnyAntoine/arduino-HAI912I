//
// Created by launa on 15/01/2026.
//
#ifndef UNTITLED_LIGHTSERVICE_H
#define UNTITLED_LIGHTSERVICE_H
#include <string>

#include "SensorService.h"


class LightService : public SensorService{
    protected:
    int lightPosition;
    float value;
    float oldValue;
    std::string unit;
    int id;
    std::string name;

    LightService();
    public:
    LightService(LightService &other) = delete;
    void operator = (const LightService &) = delete;
    float readSensor() override;
    std::string getName() const override;
    int getId() const override;
    float getOldValue() const override;
    std::string getUnit() const override;
    int getSensorPos() const override;
    static LightService *getInstance();

};


#endif //UNTITLED_LIGHTSERVICE_H