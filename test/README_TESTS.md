# Tests Unitaires

Ce dossier contient les tests unitaires pour le projet TTGO-Meteo.

## Structure des Tests

```
test/
├── test_string_utils/       # Tests des fonctions utilitaires de manipulation de strings
├── test_led_service/        # Tests de la logique de gestion des LEDs et seuils
├── test_route/              # Tests de la classe Route (API)
├── test_button_service/     # Tests de la detection des boutons
└── README.md               # Ce fichier
```

## Modules Testes

### 1. StringUtils (`test_string_utils`)
**Fonctions testees:**
- `splitIds()` - Separation de string en tableau d'IDs
- `toInt()` - Conversion string vers entier

**Tests couverts:**
- Valeur unique
- Valeurs multiples
- Gestion des espaces
- Separateurs personnalises
- Strings vides
- Valeurs negatives et zero

### 2. LedService (`test_led_service`)
**Fonctionnalites testees:**
- Allumage/extinction de la LED
- Bascule d'etat (toggle)
- Configuration des seuils
- Logique des modes de seuil:
  - Mode 0: Superieur a
  - Mode 1: Inferieur a
  - Mode 2: Switch (bascule au passage)

**Tests couverts:**
- 13 scenarios de test
- Validation des 3 modes de seuil
- Gestion d'etat
- Configuration et suppression de seuils

### 3. Route (`test_route`)
**Fonctionnalites testees:**
- Creation de routes API
- Methodes HTTP (GET, POST, PUT, DELETE, PATCH)
- Conversion methode vers string
- Gestion des descriptions

**Tests couverts:**
- Tous les types de methodes HTTP
- Chemins simples et complexes
- Descriptions vides et longues
- Cas limites (chemins vides)

### 4. ButtonService (`test_button_service`)
**Fonctionnalites testees:**
- Detection d'appui court
- Detection d'appui long (>1000ms)
- Debounce (anti-rebond de 50ms)
- Gestion des deux boutons

**Tests couverts:**
- Appuis courts gauche/droite
- Appuis longs gauche/droite
- Debounce
- Appuis multiples
- Bouton maintenu

## Execution des Tests

### Tous les tests
```bash
pio test
```

### Test specifique
```bash
pio test -f test_string_utils
pio test -f test_led_service
pio test -f test_route
pio test -f test_button_service
```

### Tests en mode verbeux
```bash
pio test -v
```

### Tests sur l'environnement natif (sans hardware)
```bash
pio test -e native
```

## Ajouter de Nouveaux Tests

1. Creer un nouveau dossier dans `test/` avec le prefixe `test_`
2. Creer un fichier `.cpp` avec les tests
3. Utiliser le framework Unity:

```cpp
#include <unity.h>

void test_ma_fonction() {
    TEST_ASSERT_EQUAL(expected, actual);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_ma_fonction);
    UNITY_END();
}

void loop() {}
```

## Assertions Disponibles

- `TEST_ASSERT_EQUAL(expected, actual)` - Egalite
- `TEST_ASSERT_NOT_EQUAL(expected, actual)` - Inegalite
- `TEST_ASSERT_TRUE(condition)` - Vrai
- `TEST_ASSERT_FALSE(condition)` - Faux
- `TEST_ASSERT_NULL(pointer)` - Pointeur null
- `TEST_ASSERT_NOT_NULL(pointer)` - Pointeur non-null
- `TEST_ASSERT_EQUAL_STRING(expected, actual)` - Strings egales
- `TEST_ASSERT_EQUAL_FLOAT(expected, actual, delta)` - Floats egaux

Documentation complete: https://docs.platformio.org/en/latest/advanced/unit-testing/

## Couverture des Tests

**Composants testes:** 4/10 (40%)
- ✅ StringUtils
- ✅ LedService
- ✅ Route
- ✅ ButtonService
- ⏳ SensorManager (a ajouter)
- ⏳ DisplayService (necessite mock TFT)
- ⏳ APIServer (integration test)
- ⏳ OTAManager (integration test)
- ⏳ SensorService (a ajouter)
- ⏳ Application (integration test)

## Bonnes Pratiques

1. **Isolation**: Chaque test doit etre independant
2. **Mock**: Utiliser des mocks pour les dependances hardware
3. **Nomenclature**: `test_[composant]_[scenario]`
4. **Setup/TearDown**: Reinitialiser l'etat entre les tests
5. **Documentation**: Commenter les scenarios complexes

## Resultats Attendus

Tous les tests doivent passer avec le message:
```
OK (X tests, Y assertions)
```

En cas d'echec, le framework Unity indique:
- Le test qui a echoue
- La ligne de l'assertion
- Les valeurs attendues vs reelles

