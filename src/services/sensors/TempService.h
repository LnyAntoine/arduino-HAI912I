//
// Created by launa on 15/01/2026.
//

#ifndef UNTITLED_TEMPSERVICE_H
#define UNTITLED_TEMPSERVICE_H
#include <string>

#include "SensorService.h"


class TempService : public SensorService {
    protected:
    TempService();
    float value;
    float oldValue;
    int tempPosition;
    std::string unit;
    int id;
    std::string name;
    public:
    TempService(TempService &other) = delete;
    void operator = (const TempService &) = delete;
    float readSensor() override;
    int getSensorPos() const override;
    std::string getName() const override;
    int getId() const override;
    float getOldValue() const override;
    std::string getUnit() const override;
    static TempService* getInstance();
    float getValue() override {return value;}

};


#endif //UNTITLED_TEMPSERVICE_H