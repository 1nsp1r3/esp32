# Prérequis
- Visual Studio Code
- ESP-IDF v5.1.2

# Clean du projet
- Supprimer les fichiers `sdkconfig`, `sdkconfig.old`, `dependencies.lock` & les répertoires `build/` & `.vscode/`

# Setup
- Lancer Visual Studio Code
- Dans la barre de statut, cliquer sur "esp32" (ESP-IDF: Set Espressif Device Target)
  - Choisir esp32s3 puis ESP32-S3 chip (via ESP-PROG)
- Ouvrir menuconfig : `F1>ESP-IDF: SDK Configuration editor (Menuconfig)`
- Dans `Component config>Bluetooth`
  - Cocher Bluetooth
  - Host: `Bluedroid - Dual-mode`
  - Controller: `Enabled`
  - Ne pas cocher "Enable BLE 5.0 features"
  - Cocher "Enable BLE 4.2 features"
- Sauver cette configuration
- Lancer un 1er build (c'est très long)

# BOSCH sensor
- References: 0261230340 / 0261230482 / DSM1TF
- Datasheet https://www.efihardware.com/download/178/H-PS150G-AT-B

```
  _____
 / 1 2 \
/ 3 4 5 \
---------

1: N/C
2: Pressure (Jaune)
3: 5V (Rouge)
4: GND (Noir)
5: Temperature (Blanc)
```

# Diagram
```
                                                __________________________________
                                               | ESP32-S3-WROOM1                  |
                                               |                                  |
    o------------------------------------------X 3.3V                             |
    |                                          |                                  |
 ___|_____________                             |                                  |
|  _|_            |                            |                                  |
| |   |           |                            |                                  |
| | R | 1800 Ohms |                            |                                  |
| | 1 |           |                            |                                  |
| |___|           |                            |                                  |
|   |             |                            |                                  |
|   o-------------|----------------------------X ADC2_5 | GPIO16                  |
|___|_____________|                            | Temperature                      |
    |                                          | 0.13V (140°) to 3.17V (-40°)     |
    o-------------------X PIN5 Temperature     |                                  |
    |       o-----------X PIN4 GND             |                                  |
 ___|___    |           X PIN3 5V              |                                  |
|  _|_  |   |      o----X PIN2 Pressure        |                                  |
| |   | |   |      |                           |                                  |
| | R | |   |      |                           |                                  |
| | 2 | |   |      |                           |                                  |
| |___| |   |      |                           |                                  |
|___|___|   |      |                           |                                  |
    |       |      |                           |                                  |
    |       |      |                           |                                  |
    |       |      |                           |                                  |
    |       |      |                           |                                  |
    |       |      |                           |                                  |
    |       |      |                           |                                  |
    |       |   ___|_____________              |                                  |
    |       |  |  _|_            |             |                                  |
    |       |  | |   |           |             |                                  |
    |       |  | | R | 1200 Ohms |             |                                  |
    |       |  | | 1 |           |             |                                  |
    |       |  | |___|           |             |                                  |
    |       |  |   |             |             |                                  |
    |       |  |   o-------------|-------------X ADC2_6 | GPIO17                  |
    |       |  |  _|_            |             | Pressure                         |
    |       |  | |   |           |             | 0.37V (0 PSI) to 3.30V (145 PSI) |
    |       |  | | R | 3300 Ohms |             |                                  |
    |       |  | | 2 |           |             |                                  |
    |       |  | |___|           |             |                                  |
    |       |  |___|_____________|             |                                  |
    |       |      |                           |                                  |
    o-------o------o---------------------------X GND                              |
                                               |__________________________________|
```

# Temperature signal
> Cf. /doc/temperature.ods

Example with 2480 Ohms for 20°C
```

Vout = (Vin x R2) / (R1 + R2)
     = (3.3 x 2480 ) / (1800 + 2480)
     = 1.91V (20°C)
```

# Pressure signal
> Cf. /doc/pressure.ods

Example with 4.5V for 145 PSI
```
Vout = (Vin x R2) / (R1 + R2)
     = (4.5 x 3300 ) / (1200 + 3300)
     = 3.3V (145 PSI)
```
