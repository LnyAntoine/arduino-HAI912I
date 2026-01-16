#include <unity.h>
#include <Arduino.h>
#include "services/sensors/SensorManager.h"
#include "services/sensors/SensorService.h"


void setUp() {
    // Preparation
}

void tearDown() {
    // Nettoyage
}

// Test que SensorManager est un singleton
void test_sensor_manager_singleton() {
    SensorManager* manager1 = SensorManager::getInstance();
    SensorManager* manager2 = SensorManager::getInstance();

    TEST_ASSERT_EQUAL_PTR(manager1, manager2);
}

// Test recuperation de tous les capteurs
void test_get_all_sensors() {
    SensorManager* manager = SensorManager::getInstance();

    const std::vector<SensorService*>& sensors = manager->getAllSensors();

    // Doit avoir au moins 2 capteurs (Light et Temp enregistres dans le constructeur)
    TEST_ASSERT_GREATER_OR_EQUAL(2, sensors.size());
}

// Test recuperation d'un capteur par ID
void test_get_sensor_by_id() {
    SensorManager* manager = SensorManager::getInstance();

    SensorService* sensor = manager->getSensorById(0);

    TEST_ASSERT_NOT_NULL(sensor);
    TEST_ASSERT_EQUAL(0, sensor->getId());
}

// Test recuperation de plusieurs capteurs par IDs
void test_get_sensors_by_ids() {
    SensorManager* manager = SensorManager::getInstance();

    std::vector<int> ids = {0, 1};
    std::vector<SensorService*> sensors = manager->getSensorsByIds(ids);

    TEST_ASSERT_EQUAL(2, sensors.size());
    TEST_ASSERT_NOT_NULL(sensors[0]);
    TEST_ASSERT_NOT_NULL(sensors[1]);
}

// Test recuperation avec liste d'IDs vide
void test_get_sensors_empty_ids() {
    SensorManager* manager = SensorManager::getInstance();

    std::vector<int> ids;
    std::vector<SensorService*> sensors = manager->getSensorsByIds(ids);

    TEST_ASSERT_EQUAL(0, sensors.size());
}

// Test recuperation avec un seul ID
void test_get_sensors_single_id() {
    SensorManager* manager = SensorManager::getInstance();

    std::vector<int> ids = {0};
    std::vector<SensorService*> sensors = manager->getSensorsByIds(ids);

    TEST_ASSERT_EQUAL(1, sensors.size());
    TEST_ASSERT_NOT_NULL(sensors[0]);
    TEST_ASSERT_EQUAL(0, sensors[0]->getId());
}

// Test que les capteurs ont des IDs uniques
void test_sensors_unique_ids() {
    SensorManager* manager = SensorManager::getInstance();

    const std::vector<SensorService*>& sensors = manager->getAllSensors();

    // Verifier que chaque capteur a un ID different
    for (size_t i = 0; i < sensors.size(); i++) {
        TEST_ASSERT_EQUAL(i, sensors[i]->getId());
    }
}

// Test que tous les capteurs ont un nom
void test_sensors_have_names() {
    SensorManager* manager = SensorManager::getInstance();

    const std::vector<SensorService*>& sensors = manager->getAllSensors();

    for (SensorService* sensor : sensors) {
        TEST_ASSERT_NOT_NULL(sensor);
        TEST_ASSERT_GREATER_THAN(0, sensor->getName().length());
    }
}

// Test que tous les capteurs ont une unite
void test_sensors_have_units() {
    SensorManager* manager = SensorManager::getInstance();

    const std::vector<SensorService*>& sensors = manager->getAllSensors();

    for (SensorService* sensor : sensors) {
        TEST_ASSERT_NOT_NULL(sensor);
        TEST_ASSERT_GREATER_THAN(0, sensor->getUnit().length());
    }
}

// Test lecture de valeurs des capteurs
void test_sensors_readable() {
    SensorManager* manager = SensorManager::getInstance();

    const std::vector<SensorService*>& sensors = manager->getAllSensors();

    for (SensorService* sensor : sensors) {
        TEST_ASSERT_NOT_NULL(sensor);
        // La lecture ne doit pas crasher
        float value = sensor->readSensor();
        // La valeur doit etre un nombre valide (pas NaN)
        TEST_ASSERT_TRUE(value == value); // NaN != NaN
    }
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_sensor_manager_singleton);
    RUN_TEST(test_get_all_sensors);
    RUN_TEST(test_get_sensor_by_id);
    RUN_TEST(test_get_sensors_by_ids);
    RUN_TEST(test_get_sensors_empty_ids);
    RUN_TEST(test_get_sensors_single_id);
    RUN_TEST(test_sensors_unique_ids);
    RUN_TEST(test_sensors_have_names);
    RUN_TEST(test_sensors_have_units);
    RUN_TEST(test_sensors_readable);

    UNITY_END();
}

void loop() {
    // Rien a faire
}

