idf_component_register(
  SRCS i3-wifi.cpp
  INCLUDE_DIRS .
  REQUIRES
    nvs_flash
    esp_wifi
)
