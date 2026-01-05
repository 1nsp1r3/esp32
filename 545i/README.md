# TODO
- Splashsrceen
- Optimiser les marges sur le LCD
- Déplacer le buffer coté i3lcd
- Rafraichir plus souvent l'écran (pour le timer) mais faire une mesure adc tous les x secondes

# Diagram
```
       _________________________________________________________________
      |                                                                 |
      |                  ESP32-S3-WAVESHARE                             |
      |                   ______________                                |
(Vin) o--ADC1_CH1|GPIO2-x| 1           1|x-3V3 (Vout)---------------o   |
         ADC1_CH3|GPIO4  | 2           2|x-GND----------------o    _|_  |
         ADC1_CH5|GPIO6  | 3           3|  GPIO43             |   |   | |
         ADC2_CH5|GPIO16 | 4           4|  GPIO44             |   | R | | R1=15000 Ω
         ADC2_CH6|GPIO17 | 5           5|  GPIO47             |   | 1 | | 
         ADC2_CH7|GPIO18 | 6           6|  GPIO48             |   |___| |
                  GPIO21 | 7           7|  GPIO15|ADC2_CH4    |     |___|
         ADC1_CH7|GPIO8  | 8           8|  GPIO13|ADC2_CH2    |    _|_
         ADC1_CH6|GPIO7  | 9           9|  GPIO11|ADC2_CH0    |   |   |
         ADC1_CH9|GPIO10 |10          10|  GPIO12|ADC2_CH1    |   | T | Tbest temperature sensor
         ADC2_CH9|GPIO20 |11          11|  GPIO14|ADC2_CH3    |   |___|
         ADC2_CH8|GPIO19 |12          12|  GPIO9 |ADC1_CH8    |_____|
                     GND |13          13|  GND
                     5V0 |14          14|  Vbat
                         |____USB-C_____|
```

# Current consumption
- 0.2mA at 0°C `3.3V/(15020 + 1250)`
- 0.02mA at 150°C `3.3V/(15020 + 162050)`

# Compilation
Target must be ESP32-S3, not ESP32

# Prompt
```
Tu es dans un projet en langage C utilisant le framework esp-idf pour programmer un esp32s3.
Il y a un composant i3-lcd, simple, qui permet de faire de la programmation graphique sur un écran.
Je voudrais que tu fasses évoluer ce composant pour pouvoir afficher des chiffres ou des nombres sur l'écran.
Choisis ce qui est le plus simple d'un point de vue quantité de code pour la police de caractère :
Soit la générer à la volée, soit la stocker directement dans le code source sous forme d'array par exemple.
```
