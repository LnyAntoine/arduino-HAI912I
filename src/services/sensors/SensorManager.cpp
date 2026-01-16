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
    return sensors[id];
}

// Recupere plusieurs capteurs par leurs IDs
std::vector<SensorService *> SensorManager::getSensorsByIds(const std::vector<int> &ids) const {
    std::vector<SensorService *> tempSensors;
    for (int i = 0; i < ids.size(); i++) {
        tempSensors.push_back(getSensorById(ids[i]));
    }
    return tempSensors;
}

// Enregistre un nouveau capteur
void SensorManager::registerSensor(SensorService* sensor) {
    sensors.push_back(sensor);
}


