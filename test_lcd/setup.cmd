rmdir /Q /S components\i3-lcd
rmdir /Q /S components\i3-lzw
rmdir /Q /S components\i3-sprite
rmdir /Q /S components\i3-sys

xcopy /Y ..\components\i3-lcd\    components\i3-lcd\
xcopy /Y ..\components\i3-lzw\    components\i3-lzw\
xcopy /Y ..\components\i3-sprite\ components\i3-sprite\
xcopy /Y ..\components\i3-sys\    components\i3-sys\
