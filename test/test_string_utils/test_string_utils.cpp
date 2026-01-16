#include <unity.h>
#include <Arduino.h>
#include "utils/StringUtils.h"

// Test de splitIds avec une seule valeur
void test_splitIds_single_value() {
    String input = "42";
    std::vector<int> result = StringUtils::splitIds(input);

    TEST_ASSERT_EQUAL(1, result.size());
    TEST_ASSERT_EQUAL(42, result[0]);
}

// Test de splitIds avec plusieurs valeurs
void test_splitIds_multiple_values() {
    String input = "1,2,3,4,5";
    std::vector<int> result = StringUtils::splitIds(input);

    TEST_ASSERT_EQUAL(5, result.size());
    TEST_ASSERT_EQUAL(1, result[0]);
    TEST_ASSERT_EQUAL(2, result[1]);
    TEST_ASSERT_EQUAL(3, result[2]);
    TEST_ASSERT_EQUAL(4, result[3]);
    TEST_ASSERT_EQUAL(5, result[4]);
}

// Test de splitIds avec des espaces
void test_splitIds_with_spaces() {
    String input = " 10 , 20 , 30 ";
    std::vector<int> result = StringUtils::splitIds(input);

    TEST_ASSERT_EQUAL(3, result.size());
    TEST_ASSERT_EQUAL(10, result[0]);
    TEST_ASSERT_EQUAL(20, result[1]);
    TEST_ASSERT_EQUAL(30, result[2]);
}

// Test de splitIds avec separateur personnalise
void test_splitIds_custom_separator() {
    String input = "7;8;9";
    std::vector<int> result = StringUtils::splitIds(input, ';');

    TEST_ASSERT_EQUAL(3, result.size());
    TEST_ASSERT_EQUAL(7, result[0]);
    TEST_ASSERT_EQUAL(8, result[1]);
    TEST_ASSERT_EQUAL(9, result[2]);
}

// Test de splitIds avec string vide
void test_splitIds_empty_string() {
    String input = "";
    std::vector<int> result = StringUtils::splitIds(input);

    TEST_ASSERT_EQUAL(1, result.size());
    TEST_ASSERT_EQUAL(0, result[0]);
}

// Test de toInt avec valeur positive
void test_toInt_positive() {
    String input = "123";
    int result = StringUtils::toInt(input);

    TEST_ASSERT_EQUAL(123, result);
}

// Test de toInt avec valeur negative
void test_toInt_negative() {
    String input = "-456";
    int result = StringUtils::toInt(input);

    TEST_ASSERT_EQUAL(-456, result);
}

// Test de toInt avec zero
void test_toInt_zero() {
    String input = "0";
    int result = StringUtils::toInt(input);

    TEST_ASSERT_EQUAL(0, result);
}

void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_splitIds_single_value);
    RUN_TEST(test_splitIds_multiple_values);
    RUN_TEST(test_splitIds_with_spaces);
    RUN_TEST(test_splitIds_custom_separator);
    RUN_TEST(test_splitIds_empty_string);
    RUN_TEST(test_toInt_positive);
    RUN_TEST(test_toInt_negative);
    RUN_TEST(test_toInt_zero);

    UNITY_END();
}

void loop() {
    // Rien a faire
}

