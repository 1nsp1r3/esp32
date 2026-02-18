idf_component_register(
  SRCS main.cpp
  INCLUDE_DIRS .
  REQUIRES
    i3-lcd
    i3-adc
    i3-steinhart
    i3-queue
    i3-lzw
    i3-sprite
    i3-sys
    i3-linear
)
