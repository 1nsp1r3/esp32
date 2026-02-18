# Diagram
```

                    (2) Bosch pressure sensor
                    _|_ 
                   |   |   _________________________________________________________________
            1200 Ω | R |  |                                                                 |
                   | 3 |  |                  ESP32-S3-WAVESHARE                             |
                   |___|  |                   ______________                                |
                     |    o--ADC1_CH1|GPIO2--| 1           1|--3V3 (Vout)---------------o   |
                     o-------ADC1_CH3|GPIO4--| 2           2|  GND                     _|_  |
                     |       ADC1_CH5|GPIO6  | 3           3|  GPIO43                 |   | |
                     |       ADC2_CH5|GPIO16 | 4           4|  GPIO44          1800 Ω | R | |
                     |       ADC2_CH6|GPIO17 | 5           5|  GPIO47                 | 1 | | 
                     |       ADC2_CH7|GPIO18 | 6           6|  GPIO48                 |___| |
                    _|_               GPIO21 | 7           7|  GPIO15|ADC2_CH4          |___|
                   |   |     ADC1_CH7|GPIO8  | 8           8|  GPIO13|ADC2_CH2          |
            3300 Ω | R |     ADC1_CH6|GPIO7  | 9           9|  GPIO11|ADC2_CH0         (5) Bosch temperature sensor
                   | 2 |     ADC1_CH9|GPIO10 |10          10|  GPIO12|ADC2_CH1         _|_ 
                   |___|     ADC2_CH9|GPIO20 |11          11|  GPIO14|ADC2_CH3        |   |
                     |       ADC2_CH8|GPIO19 |12          12|  GPIO9 |ADC1_CH8        | T |
                     o-------------o-----GND-|13          13|--GND----------------o   |___|
                                   | o---5V0-|14          14|  Vbat               |_____|
                                   | |       |____USB-C_____|                           |
                                   | |                                                 (4) Bosch GND
                                   | |
                                   | o-------------------------------------------------(3) Bosch 5V
                                   | |
                           (Power supply from car)
```

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

# Current consumption
## Temperature
- 3.17V/0.00007mA at -40°C `3.3V/(44864 + 1800)`
- 0.13V/0.002mA at 140°C `3.3V/(72 + 1800)`

## Pressure
- 3.30V/0.001mA at 145 PSI `3.3V/3300Ω`
- 0.37V/0.0001mA at 0 PSI `0.37/3300Ω`

# Compilation
- Visual Studio Code
- ESP-IDF v5.1.2

Dans la barre de statut de Visual Studio Code, cliquer sur "esp32" (ESP-IDF: Set Espressif Device Target) puis choisir `esp32s3` puis `ESP32-S3 chip (via ESP-PROG)`

## Enable PSRAM
- ESP-IDF: SDK Configuration Editor (Menuconfig)
  - Component config>ESP PSRAM
    - Support for external, SPI-connected RAM
    - Mode (QUAD/OCT) of SPI RAM chip in use: Octal Mode PSRAM
    - Make RAM allocatable using heap_caps_malloc(..., MALLOC_CAP_SPIRAM)
