# Build project
## Pré-requis pour le build
Vérifier que la target est bien esp32s3 et non pas esp32 (Dans la barre de statut)

## Components
Pour pouvoir compiler avec mes components, éditer `CMakeLists.txt` et ajouter
 
```
set(
  EXTRA_COMPONENT_DIRS ../components
)

# For i3-http
set(
  QMAKE_CXXFLAGS ${QMAKE_CXXFLAGS} -Wno-missing-field-initializers
)

```

Pour pouvoir compiler le component `i3-ble` qui a besoin de `esp_bt.h`:
- F1>ESP-IDF: SDK Configuration editor (Menuconfig)
- Dans `Component config>Bluetooth`
  - Cocher Bluetooth
  - Host: `Bluedroid - Dual-mode`
  - Controller: `Enabled`
  - Ne pas cocher "Enable BLE 5.0 features" (Si l'option n'est disponible, vérifiez que la target est bien esp32s3)
  - Cocher "Enable BLE 4.2 features"

## Ajout de LVGL
```sh
cd C:\src\esp32\components\
git submodule add https://github.com/lvgl/lvgl.git lvgl
git submodule add https://github.com/lvgl/lvgl_esp32_drivers.git lvgl_esp32_drivers
```

Editer `main/CMakeLists.txt` et ajouter
```
REQUIRES
  lvgl
  lvgl_esp32_drivers
```

F1>Build project (c'est très long)
