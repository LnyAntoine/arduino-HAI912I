#ifndef CONFIG_H
#define CONFIG_H

// ====================================
// Configuration des Pins
// ====================================
#define LED_PIN 2
#define LIGHT_PIN 32
#define TEMP_PIN 33
// ====================================
// Configuration Capteurs
// ====================================

#define VOLTAGE 3.3
#define TEMP_RES 10000.0
#define LIGHT_RES 10000.0

#define THERMISTORNOMINAL 10000.0
#define TEMPERATURENOMINAL 25.0
#define BCOEFFICIENT 3950.0
// ====================================
// Configuration API
// ====================================
#define API_PORT 80

// ====================================
// Configuration écran TFT
// ====================================
#define TFT_BL          4  // Backlight
#define BUTTON_LEFT     0  // Bouton gauche
#define BUTTON_RIGHT    35 // Bouton droit

#endif // CONFIG_H