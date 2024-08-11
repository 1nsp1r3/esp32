D# TODO
- Donner un nom spécifique à ma MX5 pour ne pas me connecter malencontrueusement à une autre

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
    o--------------------------------------------------------X 3.3V (out)                       |
    |                                                        |                                  |
 ___|_____________                                           |                                  |
|  _|_            |                                          |                                  |
| |   |           |                                          |                                  |
| | R | 1800 Ohms |                                          |                                  |
| | 1 |           |                                          |                                  |
| |___|           |                                          |                                  |
|   |             |                                          |                                  |
|   o-------------|------------------------------------------X ADC2_5 | GPIO16                  |
|___|_____________|                                          | Temperature                      |
    |                                                        | 0.13V (140°) to 3.17V (-40°)     |
    o-------------------X PIN5 Temperature                   |                                  |
    |       o-----------X PIN4 GND ----------------o         |                                  |
 ___|___    |           X PIN3 5V  -----------o    |         |                                  |
|  _|_  |   |      o----X PIN2 Pressure       |    |         |                                  |
| |   | |   |      |                          |    |         |                                  |
| | R | |   |      |                          |    |         |                                  |
| | 2 | |   |      |                          |    |         |                                  |
| |___| |   |      |                         _|____|__       |                                  |
|___|___|   |      |                        | |   GND |      |                                  |
    |       |      |                        | |       |      |                                  |
    |       |      |                        | | +5V   |      |                                  |
    |       |      |                        | o-------|------X 5V (in)                          |
    |       |      |                        |_________|      |                                  |
    |       |      |                                         |                                  |
    |       |      |                                         |                                  |
    |       |   ___|_____________                            |                                  |
    |       |  |  _|_            |                           |                                  |
    |       |  | |   |           |                           |                                  |
    |       |  | | R | 1200 Ohms |                           |                                  |
    |       |  | | 1 |           |                           |                                  |
    |       |  | |___|           |                           |                                  |
    |       |  |   |             |                           |                                  |
    |       |  |   o-------------|---------------------------X ADC2_6 | GPIO17                  |
    |       |  |  _|_            |                           | Pressure                         |
    |       |  | |   |           |                           | 0.37V (0 PSI) to 3.30V (145 PSI) |
    |       |  | | R | 3300 Ohms |                           |                                  |
    |       |  | | 2 |           |                           |                                  |
    |       |  | |___|           |                           |                                  |
    |       |  |___|_____________|                           |                                  |
    |       |      |                                         |                                  |
    o-------o------o-----------------------------------------X GND                              |
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

# Espruino simulator
```js
let rawData = [
  0x02,0x01,0x06,                //AD1: Flags
  0x04,0x09,0x4D,0x58,0x35,      //AD2: Complete Local Name "MX5"
  0x05,0x16,0x09,0x18,0x00,0x00, //AD3: Service Data 16-bit UUID 1809 (Health Thermometer Service)
  0x05,0x16,0x6D,0x2A,0x00,0x00, //AD4: Service Data 16-bit UUID 2A6D (Pressure characteristic)
]

const random = (min, max) => min + Math.floor(Math.random() * (max-min))

const longToByteArray = function(short){
    let ret = [0, 0]

    for (let i=0;i<2;i++){
        let byte = short & 0xff
        ret[i] = byte
        short = (short - byte) / 256
    }
    return ret
}

function loop(){
  let temperature = random(2000, 3000)
  let pressure = random(4500, 7500)
    
  let tmp = longToByteArray(temperature)
  rawData[12] = tmp[0]
  rawData[13] = tmp[1]
  
  tmp = longToByteArray(pressure)
  rawData[18] = tmp[0]
  rawData[19] = tmp[1]

  NRF.setAdvertising(
    rawData, {
      name: "MX5",
      interval: 1000, //ms
    }
  )
}

setInterval(loop, 1000)
loop()
```

# Temperature table when R1=1788 Ohms (Julien)
```c
int table[TABLE_SIZE][2] = {
  { 144, 140}, //0
  { 177, 130},
  { 231, 120},
  { 295, 110},
  { 368, 100},
  { 471,  90}, //5
  { 615,  80},
  { 780,  70},
  { 987,  60},
  {1263,  50},
  {1605,  40}, //10
  {1985,  30},
  {2395,  20},
  {2783,  10},
  {3165,   0},
  {3515, -10}, //15
  {3860, -20},
  {4090, -30},
  {4095, -40}, //18
};
```

# Temperature table when R1=1774 Ohms (Régis)
```c
int table[TABLE_SIZE][2] = {
  { 147, 140}, //0
  { 183, 130},
  { 233, 120},
  { 291, 110},
  { 363, 100},
  { 471,  90}, //5
  { 605,  80},
  { 771,  70},
  { 989,  60},
  {1268,  50},
  {1594,  40}, //10
  {1968,  30},
  {2352,  20},
  {2752,  10},
  {3124,   0},
  {3499, -10}, //15
  {3820, -20},
  {4095, -30},
  {4095, -40}, //18
};
```
