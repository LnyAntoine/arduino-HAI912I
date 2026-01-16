# Projet TTGO-Meteo

Station meteo connectee basee sur ESP32 avec ecran TFT, capteurs et API REST.

## Materiel requis

- TTGO T-Display (ESP32 avec ecran TFT 135x240)
- Capteurs de temperature et luminosite
- LED de notification
- 2 boutons (gauche/droit)

## Installation

### Prerequisites

- PlatformIO Core ou PlatformIO IDE
- Cable USB pour programmer l'ESP32
- Port serie disponible (COM3 par defaut)

### Configuration WiFi

Avant de compiler, configurez vos identifiants WiFi dans `src/config/wifiConfig.h`:

```cpp
#define WIFI_SSID "VotreSSID"
#define WIFI_PASSWORD "VotreMotDePasse"
```

Note: Ce fichier est ignore par Git pour proteger vos identifiants.

### Compilation et upload

Compilation seule:
```bash
pio run
```

Compilation et upload sur l'ESP32:
```bash
pio run --target upload
```

Moniteur serie (pour voir les logs):
```bash
pio device monitor
```

Tout en une commande:
```bash
pio run --target upload && pio device monitor
```

### Configuration du port serie

Si votre ESP32 n'est pas sur COM3, modifiez `platformio.ini`:
```ini
upload_port = COMX
monitor_port = COMX
```

## Tests unitaires

Le projet contient 45 tests unitaires repartis en 5 suites.

### Executer tous les tests

```bash
pio test -e lilygo-t-display --upload-port COM3
```

### Executer une suite specifique

```bash
pio test -f test_string_utils -e lilygo-t-display
pio test -f test_led_service -e lilygo-t-display
pio test -f test_route -e lilygo-t-display
pio test -f test_button_service -e lilygo-t-display
pio test -f test_sensor_manager -e lilygo-t-display
```

### Suites de tests disponibles

- **test_string_utils** (8 tests): Fonctions utilitaires de parsing
- **test_led_service** (13 tests): Logique LED et gestion des seuils
- **test_route** (11 tests): Routes API et methodes HTTP
- **test_button_service** (3 tests): Service de detection des boutons
- **test_sensor_manager** (10 tests): Gestion des capteurs

## API REST

Une fois l'ESP32 connecte au WiFi, l'API REST est accessible via l'adresse IP affichee sur l'ecran.

### Routes disponibles

#### Documentation

**GET /help**
- Description: Liste toutes les routes avec leurs methodes et descriptions
- Reponse: JSON avec tableau de routes

#### LED

**POST /led/on**
- Description: Allume la LED
- Reponse: Texte de confirmation

**POST /led/off**
- Description: Eteint la LED
- Reponse: Texte de confirmation

**POST /led/toggle**
- Description: Inverse l'etat de la LED
- Reponse: JSON avec le nouvel etat

**GET /led**
- Description: Recupere l'etat actuel de la LED
- Reponse: `{"status": 0|1, "pos": <pin>}`

#### Seuils LED

**POST /led/threshold**
- Description: Configure un seuil de declenchement automatique
- Body (JSON):
  ```json
  {
    "sensor": <id_capteur>,
    "val": <valeur_seuil>,
    "mode": <0|1|2>
  }
  ```
- Modes:
  - 0: Superieur (allume si valeur > seuil)
  - 1: Inferieur (allume si valeur < seuil)
  - 2: Switch (bascule au franchissement)

**DELETE /led/threshold**
- Description: Supprime le seuil configure
- Reponse: Texte de confirmation

**GET /led/threshold**
- Description: Recupere la configuration du seuil
- Reponse: `{"sensor": <id>, "val": <seuil>, "mode": <mode>}`

#### Capteurs

**GET /sensor**
- Description: Recupere les donnees des capteurs
- Parametres optionnels:
  - `?id=<n>`: Capteur specifique
  - `?ids=<n1,n2,n3>`: Plusieurs capteurs
  - Sans parametre: Tous les capteurs
- Reponse: 
  ```json
  {
    "sensors": [
      {
        "id": 0,
        "name": "Luminosite",
        "val": 512.5,
        "unit": "lux",
        "pos": 34
      }
    ]
  }
  ```

#### Status global

**GET /status**
- Description: Recupere plusieurs informations combinees
- Parametres:
  - `?led=1`: Infos LED
  - `?sensor_ids=<n1,n2>`: Capteurs specifiques
  - `?threshold=1`: Config seuil
- Reponse: JSON avec les infos demandees

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

Le projet suit une architecture MVP (Model-View-Presenter) adaptee pour systemes embarques.

```
src/
├── main.cpp                    Point d'entree minimal
├── core/
│   ├── Application.h/cpp       Orchestrateur principal (Presenter)
├── services/
│   ├── display/
│   │   └── DisplayService      Gestion de l'ecran TFT (View)
│   ├── input/
│   │   └── ButtonService       Detection des boutons
│   ├── output/
│   │   └── LedService          Controle LED et seuils
│   └── sensors/
│       ├── SensorManager       Gestionnaire de capteurs (Model)
│       ├── SensorService       Interface capteur
│       ├── LightService        Capteur de luminosite
│       └── TempService         Capteur de temperature
├── network/
│   ├── api/
│   │   ├── APIServer           Serveur HTTP REST
│   │   ├── HandlerList         Liste des routes
│   │   └── Route               Classe route avec handler
│   └── OTA/
│       └── OTAManager          Mises a jour OTA
├── utils/
│   └── StringUtils             Fonctions utilitaires
└── config/
    ├── config.h                Pins et constantes
    └── wifiConfig.h            Credentials WiFi (ignore par Git)
```

### Flux de donnees

```
Utilisateur → Boutons → ButtonService → Application
                                          ↓
Hardware → Capteurs → SensorManager → Application → DisplayService → Ecran
                                          ↓
                                       LedService → LED
                                          ↓
Reseau → APIServer → HandlerList → Services
```

## Description des classes

### Core

**Application**
Orchestrateur principal de l'application. Initialise tous les services, gere la boucle principale et coordonne les interactions entre composants.

### Services

**DisplayService**
Gere l'affichage sur l'ecran TFT. Affiche le statut WiFi, les donnees des capteurs, l'etat de la LED et les informations de seuil. Utilise un systeme de menus navigables.

**ButtonService**
Detecte les appuis sur les boutons gauche et droit avec debounce anti-rebond (50ms) et distinction entre appui court et long (1000ms). Retourne des evenements de type ButtonEvent.

**LedService**
Controle la LED et gere les seuils de declenchement automatique. Supporte trois modes: superieur, inferieur et switch (bascule). Singleton.

**SensorManager**
Gestionnaire centralise de tous les capteurs. Permet d'acceder aux capteurs par ID ou par liste d'IDs. Singleton.

**SensorService**
Interface abstraite pour tous les capteurs. Definit les methodes communes: lecture, nom, unite, position.

**LightService**
Capteur de luminosite. Herite de SensorService. Lit la valeur analogique d'un photoresistor.

**TempService**
Capteur de temperature. Herite de SensorService. Lit la temperature ambiante.

### Network

**APIServer**
Serveur HTTP REST sur port 80. Enregistre les routes depuis HandlerList et traite les requetes clientes. Utilise WebServer d'ESP32.

**HandlerList**
Contient toutes les routes de l'API avec leurs handlers (lambdas). Chaque route associe un chemin, une methode HTTP et une fonction de traitement.

**Route**
Encapsule une route API: chemin, methode HTTP, handler et description. Fournit des methodes pour recuperer ces informations et convertir la methode en string.

**OTAManager**
Gere les mises a jour Over-The-Air. Permet de flasher l'ESP32 via WiFi sans cable. Configure les callbacks de progression et d'erreur.

### Utils

**StringUtils**
Fonctions utilitaires pour la manipulation de strings. Contient splitIds (parse une liste d'IDs separees par virgule) et toInt (conversion string vers int).

## Fonctionnalites

### Navigation par boutons

- **Bouton gauche**: Menu precedent
- **Bouton droit**: Menu suivant
- **Menus disponibles**:
  - Statut WiFi et IP
  - Capteur de luminosite
  - Capteur de temperature
  - Etat de la LED
  - Configuration du seuil

### Seuils automatiques

La LED peut s'allumer/eteindre automatiquement selon les valeurs des capteurs:

- **Mode 0 (Superieur)**: LED allumee si capteur > seuil
- **Mode 1 (Inferieur)**: LED allumee si capteur < seuil
- **Mode 2 (Switch)**: LED bascule a chaque franchissement du seuil

Exemple: Allumer la LED quand la luminosite depasse 500 lux:
```bash
curl -X POST http://<ip>/led/threshold -d '{"sensor":0,"val":500,"mode":0}'
```

### Mise a jour OTA

Configurez PlatformIO pour uploader via OTA:
```bash
pio run --target upload --upload-port <ip_esp32>
```

Ou utilisez l'IDE Arduino avec l'outil ESP32 OTA.

## Dependances

- **ArduinoJson** 6.21.0: Serialisation/deserialisation JSON
- **TFT_eSPI** 2.5.43: Pilote ecran TFT
- **WiFi**: Bibliotheque ESP32 standard
- **WebServer**: Serveur HTTP ESP32
- **ArduinoOTA**: Mises a jour OTA

## Configuration materielle

### Pins utilisees

Voir `src/config/config.h` pour la configuration complete des pins.

- **Ecran TFT**: GPIO 19 (MOSI), 18 (SCLK), 5 (CS), 16 (DC), 23 (RST), 4 (BL)
- **Boutons**: GPIO 0 (gauche), 35 (droit)
- **LED**: Voir LED_PIN dans config.h
- **Capteurs**: Voir config.h pour les pins ADC

### Consommation memoire

- **RAM**: ~15% (49 Ko / 327 Ko)
- **Flash**: ~69% (906 Ko / 1310 Ko)

## Troubleshooting

### L'ESP32 ne se connecte pas au WiFi

Verifiez les identifiants dans `wifiConfig.h` et que le reseau est en 2.4GHz (pas 5GHz).

### Port COM introuvable

Installez les pilotes USB-Serial (CP210x ou CH340 selon votre carte).

### Ecran blanc ou pas d'affichage

Verifiez les pins TFT dans platformio.ini et que le backlight est allume (GPIO 4).

### API inaccessible

Verifiez l'IP affichee sur l'ecran et que vous etes sur le meme reseau WiFi.

### Tests en echec

Assurez-vous que l'ESP32 est bien connecte sur le bon port serie avant de lancer les tests.

## Licence

Ce projet est fourni tel quel pour usage educatif et personnel.

## Auteur

Developpe avec PlatformIO et ESP32 Arduino Framework.

