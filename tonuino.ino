#define DEBUG
#include "include/debug.h"

#include "include/Mp3Player.h"

//const byte buttonStartStop=A0;
//const byte buttonVolUp=A1;
//const byte buttonVolDown=A2;
//
//Button myBtn(buttonStartStop);
//

SoftwareSerial serial(2, 3);
Mp3Player mp3(serial, 4); // RX, TX, BUSY

void setup() {
    SERIALBEGIN(115200);
    DPRINTLNF("SETUP");

    int foo=1;

    DVPRINTLNF(foo);

    mp3.begin();
    DPRINTF("Status: ")
    DPRINTLN(mp3.getStatus())
}

void loop() {
    mp3.loop();
    DPRINTLNF("LOOP");
    DELAY(1000);
}
