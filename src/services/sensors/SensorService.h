//
// Created by launa on 15/01/2026.
//

#ifndef UNTITLED_SENSORSERVICE_H
#define UNTITLED_SENSORSERVICE_H
#include <string>

class SensorService {
    public:
    virtual ~SensorService() {}

    virtual float readSensor();

    virtual std::string getName() const;

    virtual int getId() const;

    virtual float getOldValue() const;

    virtual std::string getUnit() const;

    virtual int getSensorPos() const;

};
#endif //UNTITLED_SENSORSERVICE_H