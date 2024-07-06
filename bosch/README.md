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
2: Pressure
3: 5V
4: GND
5: Temperature
```

## Temperature signal
> Cf. /doc/temperature.ods

- 1: 3.3V (input)
- 2: Bosch sensor PIN5 (input)
- 3: Output 0.13V (140°) to 3.17V (-40°)

```  
    1
 ___|_____________
|  _|_            |   
| |   |           |
| |   | 1800 Ohms |
| |   |           |
| |___|           |
|   |             |
|   |-------------|-3-- ADC (OUT)
|___|_____________|
    |
    2
```
