# Librairie dédié à l'écran LCD Waveshare ESP32-S3

# Sprite
## BMP/Bit depth 8 (256 colors)
- Ouvrir l'image avec gimp et dans Image>Mode, choisir Indexed au lieu de RGB
  - Si l'image est déja en Indexed, choisir RGB puis refaite l'inverse
- Dans la fenêtre qui s'ouvre, choisir "Generate optimum palette"
- File>Export As "C source code header" (.h) file
  - header_data_cmap est la palette
    - Copier que les couleurs utilisées (On peut connaitre son index en cliquant sur la dernière couleur dans Windows>Dockable Dialogs>Colormap)
  - header_data est l'image

## Compresser les données de l'image
Utiliser `components/i3-lzw/` pour compresser les données à l'aide d'un programme C++ externe

```c
#include <cstdint>
#include "logo.h"
#include "i3-lzw.h"

/**
 *
 */
void generateArrayDeclaration(const uint8_t* tableau, size_t taille){
    printf("uint8_t data[] = {\n    ");

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
    uint8_t* result = i3Zip(logo, sizeof(logo), length);
    generateArrayDeclaration(result, length);
}
```

Pour les décompresser dans le programme esp32 :
```c
  size_t length;
  uint8_t* sprite = i3Unzip(logo, sizeof(logo), length);
```

## Old process
- Create the file with Paint (Windows)
- Use https://notisrac.github.io/FileToCArray to generate the C array (`unsigned char data[]  = {}`)
