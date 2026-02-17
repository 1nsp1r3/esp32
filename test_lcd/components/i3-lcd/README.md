# Librairie dédié à l'écran LCD Waveshare ESP32-S3

# Sprite
## BMP/Bit depth 8 (256 colors)
- Ouvrir l'image avec gimp et dans `Image>Mode`, choisir Indexed au lieu de RGB
  - Si l'image est déja en Indexed, choisir RGB puis refaite l'inverse
- Dans la fenêtre qui s'ouvre, choisir "Generate optimum palette"
- Pour afficher la palette et connaitre le nombre de couleur utilisées : `Windows>Dockable Dialogs>Colormap`
- Pour placer la couleur de fond en index 0
  - Clic droit sur la palette `Rearrange Colormap...` puis déplacer la couleur de fond en 1ère position
- File>Export As "C source code header" (.h) file
  - header_data_cmap est la palette
    - Copier que les couleurs utilisées
  - header_data est l'image que l'on peut compresser avec i3-lzw
