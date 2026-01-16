//
// Created by launa on 15/01/2026.
//

#ifndef UNTITLED_SENSORMANAGER_H
#define UNTITLED_SENSORMANAGER_H
#include <vector>

#include "SensorService.h"


class SensorManager {
public:
    // Ajoute un capteur singleton
    void registerSensor(SensorService* sensor);

    // Récupère tous les capteurs
    const std::vector<SensorService*>& getAllSensors() const;

    // Récupère un capteur par id
    SensorService* getSensorById(int id) const;

    // Récupère plusieurs capteurs par liste d'id
    std::vector<SensorService*> getSensorsByIds(const std::vector<int>& ids) const;

    // Singleton du manager
    static SensorManager* getInstance();

private:
    SensorManager();
    std::vector<SensorService*> sensors;
};



#endif //UNTITLED_SENSORMANAGER_H