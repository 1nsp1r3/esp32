Ajouter dans le `CMakeLists.txt` du projet qui va utiliser cette lib
 
```
set(
  EXTRA_COMPONENT_DIRS ../components
)

# For i3-http
set(
  QMAKE_CXXFLAGS ${QMAKE_CXXFLAGS} -Wno-missing-field-initializers
)
```
