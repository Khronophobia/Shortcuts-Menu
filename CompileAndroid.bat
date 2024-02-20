@ECHO OFF

set GEODE_MOD_ID=khronophobia.shortcuts_menu

adb start-server
geode build -p %PLATFORM%
adb push build-android32/%GEODE_MOD_ID%.geode storage/emulated/0/Android/media/com.geode.launcher/game/geode/mods
adb kill-server
pause