#include <unity.h>
#include <Arduino.h>
#include "services/input/ButtonService.h"

void setUp() {
    // Preparation
}

void tearDown() {
    // Nettoyage
}

// Test etat initial - aucun bouton presse
void test_buttons_initial_state() {
    ButtonService* buttons = ButtonService::getInstance();
    buttons->begin();

    ButtonEvent event = buttons->checkButtons();
    TEST_ASSERT_EQUAL(BUTTON_NONE, event);
}

// Test que getInstance retourne toujours la meme instance (singleton)
void test_button_service_singleton() {
    ButtonService* buttons1 = ButtonService::getInstance();
    ButtonService* buttons2 = ButtonService::getInstance();

    TEST_ASSERT_EQUAL_PTR(buttons1, buttons2);
}

// Test que checkButtons ne crash pas
void test_check_buttons_does_not_crash() {
    ButtonService* buttons = ButtonService::getInstance();
    buttons->begin();

    // Appeler plusieurs fois pour verifier la stabilite
    for(int i = 0; i < 10; i++) {
        buttons->checkButtons();
        delay(10);
    }

    TEST_ASSERT_TRUE(true); // Si on arrive ici, pas de crash
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_buttons_initial_state);
    RUN_TEST(test_button_service_singleton);
    RUN_TEST(test_check_buttons_does_not_crash);

    UNITY_END();
}

void loop() {
    // Rien a faire
}

