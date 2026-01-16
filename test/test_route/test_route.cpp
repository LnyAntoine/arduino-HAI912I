#include <unity.h>
#include <Arduino.h>
#include "network/api/Route.h"


void setUp() {
    // Preparation avant chaque test
}

void tearDown() {
    // Nettoyage apres chaque test
}

// Test creation d'une route GET
void test_route_creation_get() {
    Route route("/test", HTTP_GET, [](WebServer& server) {}, "Test route");

    TEST_ASSERT_EQUAL_STRING("/test", route.getPath().c_str());
    TEST_ASSERT_EQUAL(HTTP_GET, route.getMethod());
    TEST_ASSERT_EQUAL_STRING("Test route", route.getDescription().c_str());
}

// Test creation d'une route POST
void test_route_creation_post() {
    Route route("/api/data", HTTP_POST, [](WebServer& server) {}, "Post data");

    TEST_ASSERT_EQUAL_STRING("/api/data", route.getPath().c_str());
    TEST_ASSERT_EQUAL(HTTP_POST, route.getMethod());
    TEST_ASSERT_EQUAL_STRING("Post data", route.getDescription().c_str());
}

// Test getMethodString pour GET
void test_get_method_string_get() {
    Route route("/test", HTTP_GET, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("GET", route.getMethodString().c_str());
}

// Test getMethodString pour POST
void test_get_method_string_post() {
    Route route("/test", HTTP_POST, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("POST", route.getMethodString().c_str());
}

// Test getMethodString pour PUT
void test_get_method_string_put() {
    Route route("/test", HTTP_PUT, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("PUT", route.getMethodString().c_str());
}

// Test getMethodString pour DELETE
void test_get_method_string_delete() {
    Route route("/test", HTTP_DELETE, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("DELETE", route.getMethodString().c_str());
}

// Test getMethodString pour PATCH
void test_get_method_string_patch() {
    Route route("/test", HTTP_PATCH, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("PATCH", route.getMethodString().c_str());
}

// Test route sans description
void test_route_no_description() {
    Route route("/test", HTTP_GET, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("", route.getDescription().c_str());
}

// Test route avec chemin vide
void test_route_empty_path() {
    Route route("", HTTP_GET, [](WebServer& server) {}, "Empty path");

    TEST_ASSERT_EQUAL_STRING("", route.getPath().c_str());
    TEST_ASSERT_EQUAL_STRING("Empty path", route.getDescription().c_str());
}

// Test route avec chemin complexe
void test_route_complex_path() {
    Route route("/api/v1/sensors/temperature", HTTP_GET, [](WebServer& server) {});

    TEST_ASSERT_EQUAL_STRING("/api/v1/sensors/temperature", route.getPath().c_str());
}

// Test route avec description longue
void test_route_long_description() {
    String desc = "Cette route recupere toutes les donnees des capteurs de temperature et d'humidite";
    Route route("/sensors", HTTP_GET, [](WebServer& server) {}, desc);

    TEST_ASSERT_EQUAL_STRING(desc.c_str(), route.getDescription().c_str());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_route_creation_get);
    RUN_TEST(test_route_creation_post);
    RUN_TEST(test_get_method_string_get);
    RUN_TEST(test_get_method_string_post);
    RUN_TEST(test_get_method_string_put);
    RUN_TEST(test_get_method_string_delete);
    RUN_TEST(test_get_method_string_patch);
    RUN_TEST(test_route_no_description);
    RUN_TEST(test_route_empty_path);
    RUN_TEST(test_route_complex_path);
    RUN_TEST(test_route_long_description);

    UNITY_END();
}

void loop() {
    // Rien a faire
}

