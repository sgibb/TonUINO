#include <JC_Button.h>

#define DEBUG
#include "include/debug.h"

#include "include/Mp3Player.h"

#define INITIALVOLUME 15 // 0-30
#define MAXALLOWEDVOLUME 24 // 0-30
#define LONGPRESS 1000 // msec

Button buttonStartPause(A0);
Button buttonVolUp(A1);
Button buttonVolDown(A2);

SoftwareSerial serial(2, 3); // RX, TX
Mp3Player mp3(serial, 4); // BUSY

void setup() {
    SERIALBEGIN(115200);
    DPRINTLNF("SETUP");

    buttonStartPause.begin();
    buttonVolUp.begin();
    buttonVolDown.begin();

    mp3.begin();
    mp3.setMaxVolume(MAXALLOWEDVOLUME);
    mp3.setVolume(INITIALVOLUME);
    DVPRINTLNF(mp3.getVolume());
    DVPRINTLNF(mp3.getPlaybackMode());
    DVPRINTLNF(mp3.getTotalTrackCount());
    mp3.setFolder(2);
    DVPRINTLNF(mp3.tracks());
    mp3.playFolderTrack(2, 1);
}

void loop() {
    DPRINTLNF("LOOP");
    DVPRINTLNF(mp3.isPlaying());
    DVPRINTLNF(mp3.getStatus());
    DVPRINTLNF(mp3.getVolume());
    DVPRINTLNF(mp3.track());

    mp3.loop();

    buttonStartPause.read();
    buttonVolUp.read();
    buttonVolDown.read();

    if (buttonStartPause.wasReleased()) {
        if (mp3.isPlaying()) {
            mp3.pause();
        } else {
            mp3.start();
        }
    }

    if (buttonVolUp.pressedFor(LONGPRESS)) {
        mp3.increaseVolume();
    }
    if (buttonVolDown.pressedFor(LONGPRESS)) {
        mp3.decreaseVolume();
    }

    if (buttonVolUp.wasReleased()) {
        mp3.nextTrack();
    }
    if (buttonVolDown.wasReleased()) {
        mp3.prevTrack();
    }

    DELAY(100);
}
