#include "SensorManager.h"

#include "LightService.h"
#include "TempService.h"
class SensorManager;


// Constructeur - enregistre tous les capteurs disponibles
SensorManager::SensorManager() {
    registerSensor(LightService::getInstance());
    registerSensor(TempService::getInstance());
}

SensorManager* SensorManager::getInstance() {
    static SensorManager instance;
    return &instance;
}

// Retourne la liste de tous les capteurs
const std::vector<SensorService *> &SensorManager::getAllSensors() const {
    return sensors;
}

// Recupere un capteur par son ID
SensorService *SensorManager::getSensorById(const int id) const {
    if (id < 0 || id >= static_cast<int>(sensors.size())) {
        return nullptr;
    }
    return sensors[id];
}

// Recupere plusieurs capteurs par leurs IDs
std::vector<SensorService *> SensorManager::getSensorsByIds(const std::vector<int> &ids) const {
    std::vector<SensorService *> tempSensors;
    for (const int id : ids) {
        tempSensors.push_back(getSensorById(id));
    }
    return tempSensors;
}

// Enregistre un nouveau capteur
void SensorManager::registerSensor(SensorService* sensor) {
    sensors.push_back(sensor);
}


