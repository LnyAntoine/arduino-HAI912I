# Projet TTGO-Météo

Station météo connectée basée sur ESP32 avec écran TFT, capteurs et API REST.

## Matériel requis

- TTGO T-Display (ESP32 avec écran TFT 135x240)
- Capteurs de température et luminosité
- LED de notification
- 2 boutons (gauche/droit)

## Installation

### Prérequis

- PlatformIO Core ou PlatformIO IDE
- Câble USB pour programmer l'ESP32
- Port série disponible (COM3 par défaut)

### Configuration WiFi

Avant de compiler, configurez vos identifiants WiFi dans `src/config/wifiConfig.h` :

```cpp
#define WIFI_SSID "VotreSSID"
#define WIFI_PASSWORD "VotreMotDePasse"
```

Note : ce fichier est ignoré par Git pour protéger vos identifiants.

### Compilation et upload

Compilation seule :
```bash
pio run
```

Compilation et upload sur l'ESP32 :
```bash
pio run --target upload
```

Moniteur série (pour voir les logs) :
```bash
pio device monitor
```

Tout en une commande :
```bash
pio run --target upload && pio device monitor
```

### Configuration du port série

Si votre ESP32 n'est pas sur COM3, modifiez `platformio.ini` :
```ini
upload_port = COMX
monitor_port = COMX
```

## Tests unitaires

Le projet contient 45 tests unitaires répartis en 5 suites.

### Exécuter tous les tests

```bash
pio test -e lilygo-t-display --upload-port COM3
```

### Exécuter une suite spécifique

```bash
pio test -f test_string_utils -e lilygo-t-display
pio test -f test_led_service -e lilygo-t-display
pio test -f test_route -e lilygo-t-display
pio test -f test_button_service -e lilygo-t-display
pio test -f test_sensor_manager -e lilygo-t-display
```

### Suites de tests disponibles

- **test_string_utils** (8 tests) : Fonctions utilitaires de parsing
- **test_led_service** (13 tests) : Logique LED et gestion des seuils
- **test_route** (11 tests) : Routes API et méthodes HTTP
- **test_button_service** (3 tests) : Service de détection des boutons
- **test_sensor_manager** (10 tests) : Gestion des capteurs

## API REST

Une fois l'ESP32 connecté au WiFi, l'API REST est accessible via l'adresse IP affichée sur l'écran.

### Routes disponibles

#### Documentation

**GET /help**
- Description : Liste toutes les routes avec leurs méthodes et descriptions
- Réponse : JSON avec tableau de routes

#### LED

**POST /led/on**
- Description : Allume la LED
- Réponse : Texte de confirmation

**POST /led/off**
- Description : Éteint la LED
- Réponse : Texte de confirmation

**POST /led/toggle**
- Description : Inverse l'état de la LED
- Réponse : JSON avec le nouvel état

**GET /led**
- Description : Récupère l'état actuel de la LED
- Réponse (exemple) : `{"status": 1, "pos": 34}`  // status : 0 (éteint) ou 1 (allumé)

#### Seuils LED

**POST /led/threshold**
- Description : Configure un seuil de déclenchement automatique
- Body (JSON) — exemple :
  ```json
  {
    "sensor": 0,
    "val": 500,
    "mode": 0
  }
  ```
- Modes :
  - 0 : Supérieur (allume si valeur > seuil)
  - 1 : Inférieur (allume si valeur < seuil)
  - 2 : Switch (bascule au franchissement)

**DELETE /led/threshold**
- Description : Supprime le seuil configuré
- Réponse : Texte de confirmation

**GET /led/threshold**
- Description : Récupère la configuration du seuil
- Réponse (exemple) : `{"sensor": 0, "val": 500, "mode": 0}`

#### Capteurs

**GET /sensor**
- Description : Récupère les données des capteurs
- Paramètres optionnels :
  - `?id=<n>` : Capteur spécifique
  - `?ids=<n1,n2,n3>` : Plusieurs capteurs
  - Sans paramètre : Tous les capteurs
- Réponse :
  ```json
  {
    "sensors": [
      {
        "id": 0,
        "name": "Luminosité",
        "val": 512.5,
        "unit": "lux",
        "pos": 34
      }
    ]
  }
  ```

#### Statut global

**GET /status**
- Description : Récupère plusieurs informations combinées
- Paramètres :
  - `?led=1` : Infos LED
  - `?sensor_ids=<n1,n2>` : Capteurs spécifiques
  - `?threshold=1` : Config seuil
- Réponse : JSON avec les infos demandées

### Exemples d'utilisation

```bash
# Allumer la LED
curl -X POST http://<ip_esp32>/led/on

# Lire le capteur 0
curl http://<ip_esp32>/sensor?id=0

# Configurer un seuil
curl -X POST http://<ip_esp32>/led/threshold \
  -H "Content-Type: application/json" \
  -d '{"sensor":0,"val":500,"mode":0}'

# Lister toutes les routes
curl http://<ip_esp32>/help
```

## Architecture du projet

Le projet suit une architecture MVP (Model-View-Presenter) adaptée pour systèmes embarqués.

```
src/
├── main.cpp                    Point d'entrée minimal
├── core/
│   ├── Application.h/cpp       Orchestrateur principal (Presenter)
├── services/
│   ├── display/
│   │   └── DisplayService      Gestion de l'écran TFT (View)
│   ├── input/
│   │   └── ButtonService       Détection des boutons
│   ├── output/
│   │   └── LedService          Contrôle LED et seuils
│   └── sensors/
│       ├── SensorManager       Gestionnaire de capteurs (Model)
│       ├── SensorService       Interface capteur
│       ├── LightService        Capteur de luminosité
│       └── TempService         Capteur de température
├── network/
│   ├── api/
│   │   ├── APIServer           Serveur HTTP REST
│   │   ├── HandlerList         Liste des routes
│   │   └── Route               Classe route avec handler
│   └── OTA/
│       └── OTAManager          Mises à jour OTA
├── utils/
│   └── StringUtils             Fonctions utilitaires
└── config/
    ├── config.h                Pins et constantes
    └── wifiConfig.h            Identifiants WiFi (ignoré par Git)
```

### Flux de données

```
Utilisateur → Boutons → ButtonService → Application
                                          ↓
Hardware → Capteurs → SensorManager → Application → DisplayService → Écran
                                          ↓
                                       LedService → LED
                                          ↓
Réseau → APIServer → HandlerList → Services
```

## Description des classes

### Core

**Application**
Orchestrateur principal de l'application. Initialise tous les services, gère la boucle principale et coordonne les interactions entre composants.

### Services

**DisplayService**
Gère l'affichage sur l'écran TFT. Affiche le statut WiFi, les données des capteurs, l'état de la LED et les informations de seuil. Utilise un système de menus navigables.

**ButtonService**
Détecte les appuis sur les boutons gauche et droit avec anti-rebond (50 ms) et distinction entre appui court et long (1000 ms). Retourne des événements de type ButtonEvent.

**LedService**
Contrôle la LED et gère les seuils de déclenchement automatique. Supporte trois modes : supérieur, inférieur et switch (bascule). Singleton.

**SensorManager**
Gestionnaire centralisé de tous les capteurs. Permet d'accéder aux capteurs par ID ou par liste d'IDs. Singleton.

**SensorService**
Interface abstraite pour tous les capteurs. Définit les méthodes communes : lecture, nom, unité, position.

**LightService**
Capteur de luminosité. Hérite de SensorService. Lit la valeur analogique d'un photorésistor.

**TempService**
Capteur de température. Hérite de SensorService. Lit la température ambiante.

### Network

**APIServer**
Serveur HTTP REST sur le port 80. Enregistre les routes depuis HandlerList et traite les requêtes clientes. Utilise WebServer d'ESP32.

**HandlerList**
Contient toutes les routes de l'API avec leurs handlers (lambdas). Chaque route associe un chemin, une méthode HTTP et une fonction de traitement.

**Route**
Encapsule une route API : chemin, méthode HTTP, handler et description. Fournit des méthodes pour récupérer ces informations et convertir la méthode en chaîne.

**OTAManager**
Gère les mises à jour Over-The-Air. Permet de flasher l'ESP32 via WiFi sans câble. Configure les callbacks de progression et d'erreur.

### Utils

**StringUtils**
Fonctions utilitaires pour la manipulation de chaînes. Contient splitIds (parse une liste d'IDs séparées par virgule) et toInt (conversion string vers int).

## Fonctionnalités

### Navigation par boutons

- **Bouton gauche** : Menu précédent
- **Bouton droit** : Menu suivant
- **Menus disponibles** :
  - Statut WiFi et IP
  - Capteur de luminosité
  - Capteur de température
  - État de la LED
  - Configuration du seuil

### Seuils automatiques

La LED peut s'allumer/éteindre automatiquement selon les valeurs des capteurs :

- **Mode 0 (Supérieur)** : LED allumée si capteur > seuil
- **Mode 1 (Inférieur)** : LED allumée si capteur < seuil
- **Mode 2 (Switch)** : LED bascule à chaque franchissement du seuil

Exemple : allumer la LED quand la luminosité dépasse 500 lux :
```bash
curl -X POST http://<ip>/led/threshold -d '{"sensor":0,"val":500,"mode":0}'
```

### Mise à jour OTA

Configurez PlatformIO pour uploader via OTA :
```bash
pio run --target upload --upload-port <ip_esp32>
```

Ou utilisez l'IDE Arduino avec l'outil ESP32 OTA.

## Dépendances

- **ArduinoJson** 6.21.0 : Sérialisation/désérialisation JSON
- **TFT_eSPI** 2.5.43 : Pilote écran TFT
- **WiFi** : Bibliothèque ESP32 standard
- **WebServer** : Serveur HTTP ESP32
- **ArduinoOTA** : Mises à jour OTA

## Configuration matérielle

### Pins utilisées

Voir `src/config/config.h` pour la configuration complète des pins.

- **Écran TFT** : GPIO 19 (MOSI), 18 (SCLK), 5 (CS), 16 (DC), 23 (RST), 4 (BL)
- **Boutons** : GPIO 0 (gauche), 35 (droit)
- **LED** : Voir LED_PIN dans config.h
- **Capteurs** : Voir config.h pour les pins ADC

### Consommation mémoire

- **RAM** : ~15% (49 Ko / 327 Ko)
- **Flash** : ~69% (906 Ko / 1310 Ko)

## Dépannage

### L'ESP32 ne se connecte pas au WiFi

Vérifiez les identifiants dans `wifiConfig.h` et que le réseau est en 2.4 GHz (pas 5 GHz).

### Port COM introuvable

Installez les pilotes USB-Serial (CP210x ou CH340 selon votre carte).

### Écran blanc ou pas d'affichage

Vérifiez les pins TFT dans platformio.ini et que le rétroéclairage est allumé (GPIO 4).

### API inaccessible

Vérifiez l'IP affichée sur l'écran et que vous êtes sur le même réseau WiFi.

### Tests en échec

Assurez-vous que l'ESP32 est bien connecté sur le bon port série avant de lancer les tests.

## Licence

Ce projet est fourni tel quel pour usage éducatif et personnel.

## Auteur

Développé avec PlatformIO et le framework Arduino pour ESP32.
