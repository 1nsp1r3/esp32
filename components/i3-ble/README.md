# Liens
- [[https://www.lucadentella.it/en/2018/02/09/esp32-31-ble-gap]]
- [[https://github.com/lucadentella/esp32-tutorial/blob/master/27_ble_raw_advertise/main/main.c]]

# Compilation
Pour pouvoir compiler ce composant, qui nécessite `esp_bt.h` :

`F1>ESP-IDF: SDK Configuration editor (Menuconfig)`, dans `Component config>Bluetooth`
- Cocher Bluetooth
- Host: `Bluedroid - Dual-mode`
- Controller: `Enabled`
- Ne pas cocher "Enable BLE 5.0 features" (Si l'option n'est disponible, vérifiez que la target est bien esp32s3)
- Cocher "Enable BLE 4.2 features"
