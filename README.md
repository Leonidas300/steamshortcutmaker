# steamshortcutmaker
QT Program that generates shortcuts for steam of the lutris games.

I made 2 version released, one compiled on Zorin and one on Ubuntu but both uses
-unsupported-bundle-everything tag so may not work on older systems or you can compile it yourself using Qt.

Compile Step by step instructions:

1. Download Project Source
2. Open QTCreator and Build it.
3. Download https://github.com/probonopd/linuxdeployqt
4. In terminal -> sudo chmod a + x filename.AppImage (where filename = linuxdeploy filename)
5. In terminal -> export PATH=pathToQt/version/compiler/bin/:$PATH 
   (version replace with your qt version for example: 5.4.12 and compiler with your compiler for example gcc_64)
6. In terminal -> ./linuxdeployqt fullPathtoCompiledAppFileName -unsupported-bundle-everything -appimage
7. Enjoy. 
