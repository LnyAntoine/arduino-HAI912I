#include "SensorManager.h"

#include "LightService.h"
#include "TempService.h"
class SensorManager;


SensorManager::SensorManager() {
    registerSensor(LightService::getInstance());
    registerSensor(TempService::getInstance());

}


SensorManager* SensorManager::getInstance() {
    static SensorManager instance;
    return &instance;
}

const std::vector<SensorService *> &SensorManager::getAllSensors() const {
    return sensors;
}

SensorService *SensorManager::getSensorById(const int id) const {
    return sensors[id];
}

std::vector<SensorService *> SensorManager::getSensorsByIds(const std::vector<int> &ids) const {
    std::vector<SensorService *> tempSensors;
    for (int i = 0; i < ids.size(); i++) {
        tempSensors.push_back(getSensorById(ids[i]));
    }
    return tempSensors;
}

void SensorManager::registerSensor(SensorService* sensor) {
    sensors.push_back(sensor);
}


