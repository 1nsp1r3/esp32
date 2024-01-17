cmake_minimum_required(VERSION 3.16)

set(
  EXTRA_COMPONENT_DIRS ../components
)

set(
  QMAKE_CXXFLAGS ${QMAKE_CXXFLAGS} -Wno-missing-field-initializers
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(test_wifi)

