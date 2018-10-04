BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyACM0
MONITOR_BAUDRATE = 115200
ARDUINO_DIR = /home/sebastian/tmp/downloads/arduino-1.8.7/
USER_LIB_PATH += /home/sebastian/projects/arduino/sketchbook/libraries
ARDUINO_LIBS += SoftwareSerial DFPlayerMiniMp3 JC_Button
include /usr/share/arduino/Arduino.mk

