# Lib for data compression (LZW)
E.g. 39601 -> 2213 bytes (94.4%!)

# Exemple pour compresser des données
- Avec gimp, on peut exporter une image vers un fichier .h et obtenir ainsi une variable `unsigned char data[]`
- Ces données peuvent être compressées en utilisant `components/i3-lzw/` à l'aide d'un programme C++ externe
  - `g++ i3-lzw.cpp main.cpp -o main`

```c
#include <cstdint>
#include "logo.h" //Gimp export
#include "i3-lzw.h"

/**
 *
 */
void generateArrayDeclaration(const unsigned char* tableau, size_t taille){
    printf("unsigned char data[] = {\n    ");

    for (size_t i=0;i<taille;i++){
        printf("0x%02X", tableau[i]);
        if (i < taille-1){
            printf(",");
        }

        if ((i+1) % 12 == 0 && i < taille-1) printf("\n    "); // Retour à la ligne toutes les 12 valeurs pour une meilleure lisibilité
    }

    printf("\n};\n");
}

int main() {
    size_t length;
    uint8_t* result = i3Zip(data, sizeof(data), length);
    generateArrayDeclaration(result, length);
}
```

Pour les décompresser dans le programme esp32 :
```c
  size_t length;
  uint8_t* sprite = i3Unzip(logo, sizeof(logo), length);
```
