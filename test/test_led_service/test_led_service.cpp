#include <unity.h>
#include <Arduino.h>
#include "services/output/LedService.h"

void setUp() {
    // Reset le service avant chaque test
    LedService* led = LedService::getInstance();
    pinMode(led->getLedPos(), OUTPUT);
    led->ledOff();
    led->deleteThreshold();
}

void tearDown() {
    // Nettoyage apres chaque test
}

// Test de l'etat initial de la LED
void test_led_initial_state() {
    LedService* led = LedService::getInstance();

    TEST_ASSERT_EQUAL(0, led->getStatus());
    TEST_ASSERT_EQUAL(-1, led->getThresholdSensor());
    TEST_ASSERT_EQUAL(-1, led->getThresholdValue());
    TEST_ASSERT_EQUAL(-1, led->getThresholdMode());
}

// Test allumer la LED
void test_led_turn_on() {
    LedService* led = LedService::getInstance();

    led->ledOn();
    TEST_ASSERT_EQUAL(1, led->getStatus());
}

// Test eteindre la LED
void test_led_turn_off() {
    LedService* led = LedService::getInstance();

    led->ledOn();
    led->ledOff();
    TEST_ASSERT_EQUAL(0, led->getStatus());
}

// Test basculer l'etat de la LED
void test_led_toggle() {
    LedService* led = LedService::getInstance();

    TEST_ASSERT_EQUAL(0, led->getStatus());

    led->changeStatus();
    TEST_ASSERT_EQUAL(1, led->getStatus());

    led->changeStatus();
    TEST_ASSERT_EQUAL(0, led->getStatus());
}

// Test configuration du seuil
void test_set_threshold() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(2, 100, 0);

    TEST_ASSERT_EQUAL(2, led->getThresholdSensor());
    TEST_ASSERT_EQUAL(100, led->getThresholdValue());
    TEST_ASSERT_EQUAL(0, led->getThresholdMode());
}

// Test suppression du seuil
void test_delete_threshold() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(1, 50, 1);
    led->deleteThreshold();

    TEST_ASSERT_EQUAL(-1, led->getThresholdSensor());
    TEST_ASSERT_EQUAL(-1, led->getThresholdValue());
    TEST_ASSERT_EQUAL(-1, led->getThresholdMode());
}

// Test seuil mode superieur (mode 0) - allumage
void test_threshold_greater_than_turn_on() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 0); // Mode superieur
    led->ledOff();

    led->updateLedThreshold(60, 40); // Valeur passe au-dessus du seuil
    TEST_ASSERT_EQUAL(1, led->getStatus());
}

// Test seuil mode superieur (mode 0) - extinction
void test_threshold_greater_than_turn_off() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 0); // Mode superieur
    led->ledOn();

    led->updateLedThreshold(40, 60); // Valeur passe en-dessous du seuil
    TEST_ASSERT_EQUAL(0, led->getStatus());
}

// Test seuil mode inferieur (mode 1) - allumage
void test_threshold_less_than_turn_on() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 1); // Mode inferieur
    led->ledOff();

    led->updateLedThreshold(40, 60); // Valeur passe en-dessous du seuil
    TEST_ASSERT_EQUAL(1, led->getStatus());
}

// Test seuil mode inferieur (mode 1) - extinction
void test_threshold_less_than_turn_off() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 1); // Mode inferieur
    led->ledOn();

    led->updateLedThreshold(60, 40); // Valeur passe au-dessus du seuil
    TEST_ASSERT_EQUAL(0, led->getStatus());
}

// Test seuil mode switch (mode 2) - bascule montante
void test_threshold_switch_rising() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 2); // Mode switch
    led->ledOff();

    led->updateLedThreshold(60, 40); // Passage croissant du seuil
    TEST_ASSERT_EQUAL(1, led->getStatus());
}

// Test seuil mode switch (mode 2) - bascule descendante
void test_threshold_switch_falling() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 2); // Mode switch
    led->ledOn();

    led->updateLedThreshold(40, 60); // Passage decroissant du seuil
    TEST_ASSERT_EQUAL(0, led->getStatus());
}

// Test pas de changement si valeur ne franchit pas le seuil
void test_threshold_no_crossing() {
    LedService* led = LedService::getInstance();

    led->setSensorTreshold(0, 50, 2); // Mode switch
    led->ledOff();

    led->updateLedThreshold(45, 40); // Pas de franchissement
    TEST_ASSERT_EQUAL(0, led->getStatus());

    led->updateLedThreshold(55, 60); // Pas de franchissement
    TEST_ASSERT_EQUAL(0, led->getStatus());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_led_initial_state);
    RUN_TEST(test_led_turn_on);
    RUN_TEST(test_led_turn_off);
    RUN_TEST(test_led_toggle);
    RUN_TEST(test_set_threshold);
    RUN_TEST(test_delete_threshold);
    RUN_TEST(test_threshold_greater_than_turn_on);
    RUN_TEST(test_threshold_greater_than_turn_off);
    RUN_TEST(test_threshold_less_than_turn_on);
    RUN_TEST(test_threshold_less_than_turn_off);
    RUN_TEST(test_threshold_switch_rising);
    RUN_TEST(test_threshold_switch_falling);
    RUN_TEST(test_threshold_no_crossing);

    UNITY_END();
}

void loop() {
    // Rien a faire
}

