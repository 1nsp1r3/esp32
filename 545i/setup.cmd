rmdir /Q /S components\i3-lcd
rmdir /Q /S components\i3-adc
rmdir /Q /S components\i3-queue
rmdir /Q /S components\i3-steinhart
rmdir /Q /S components\i3-lzw
rmdir /Q /S components\i3-sprite
rmdir /Q /S components\i3-sys

xcopy /Y ..\components\i3-lcd\       components\i3-lcd\
xcopy /Y ..\components\i3-adc\       components\i3-adc\
xcopy /Y ..\components\i3-queue\     components\i3-queue\
xcopy /Y ..\components\i3-steinhart\ components\i3-steinhart\
xcopy /Y ..\components\i3-lzw\       components\i3-lzw\
xcopy /Y ..\components\i3-sprite\    components\i3-sprite\
xcopy /Y ..\components\i3-sys\       components\i3-sys\
