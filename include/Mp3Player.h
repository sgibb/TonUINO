#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>

#include "Mp3Notifier.h"

#define INITIALVOLUME 2 // 0-30
#define MAXALLOWEDVOLUME 24 // 0-30

enum Mp3Advert {
    Mp3AdvertRfidReadFailed = 1,
    Mp3AdvertPlayModus,
};

enum Mp3Messages {
    Mp3MessageApplyNewRfidCard = 256,
    Mp3MessageNewRfidCardWritten,
    Mp3MessageNewRfidCardFailed
};

class Mp3Player : public DFMiniMp3<SoftwareSerial, Mp3Notifier> {
public:
    Mp3Player(SoftwareSerial& softwareSerial, byte pinBusy)
    : DFMiniMp3<SoftwareSerial, Mp3Notifier>(softwareSerial),
        _pinBusy(pinBusy),
        _curVolume(INITIALVOLUME), _maxVolume(MAXALLOWEDVOLUME),
        _folder(0), _track(0),
        _isSleeping(false) {};

    bool isBusy() { return !digitalRead(_pinBusy); };
    void setMaxVolume(uint8_t volume) { _maxVolume = volume; };

    uint8_t folder() { return _folder; };
    void setFolder(uint8_t folder);
    uint16_t folders() { return getTotalFolderCount() - 2; } // ignore advert/mp3

    void nextFolder() { setFolder(++_folder); }
    void prevFolder() { setFolder(--_folder); }

    uint8_t track() { return _track; };
    void setTrack(uint8_t track) { _track = track; };
    uint16_t tracks() { return DFMiniMp3<SoftwareSerial, Mp3Notifier>::getFolderTrackCount(_folder); };

    void nextTrack();
    void prevTrack();

    void togglePlayPause();
    void toggleSleepState();

    // overloaded
    void decreaseVolume();
    void increaseVolume();
    void setVolume(uint8_t volume);
    void loop();

private:
    void playTrack();

    const byte _pinBusy;
    uint8_t _curVolume;
    uint8_t _maxVolume;
    uint8_t _folder;
    uint8_t _track;
    bool _isSleeping;
};

void Mp3Player::loop() {
    DFMiniMp3<SoftwareSerial, Mp3Notifier>::loop();

    if (Mp3Notifier::isTrackFinished) {
        nextTrack();
    }
}

void Mp3Player::nextTrack() {
    DPRINTLNF("nextTrack()");
    // if last track reached set to 0;
    _track = (tracks() > _track) ? _track + 1 : 0;
    DVPRINTLNF(_track);
    if (_track) {
        playTrack();
    }
}

void Mp3Player::prevTrack() {
    DPRINTLNF("prevTrack()");
    _track = max(_track - 1, 1);
    DVPRINTLNF(_track);
    playTrack();
}

void Mp3Player::playTrack() {
    DFMiniMp3<SoftwareSerial, Mp3Notifier>::playFolderTrack(_folder, _track);
    Mp3Notifier::isTrackFinished = false;
}

void Mp3Player::togglePlayPause() {
    if (isBusy() || !_folder) {
        DPRINTLNF("pause()");
        DFMiniMp3<SoftwareSerial, Mp3Notifier>::pause();
    } else {
        DFMiniMp3<SoftwareSerial, Mp3Notifier>::start();
        DPRINTLNF("start()");
    }
}

void Mp3Player::toggleSleepState() {
    DPRINTLNF("The MP3-Player nevers wakes up; feature disabled!");
    //if (_isSleeping) {
    //    DPRINTLNF("reset()");
    //    reset();
    //    DFMiniMp3<SoftwareSerial, Mp3Notifier>::playFolderTrack(_folder, _track);
    //    setVolume(_curVolume);
    //    _isSleeping = false;
    //} else {
    //    DPRINTLNF("sleep()");
    //    sleep();
    //    _isSleeping = true;
    //}
}

inline void Mp3Player::setFolder(uint8_t folder) {
    _folder = max(1, min(folder, folders()));
    DPRINTF("Set folder to ");
    DPRINTLN(_folder);
}

inline void Mp3Player::decreaseVolume() {
    setVolume(--_curVolume);
}

inline void Mp3Player::increaseVolume() {
    setVolume(++_curVolume);
}

inline void Mp3Player::setVolume(uint8_t volume) {
    if (volume <= _maxVolume) {
        DPRINTF("Set volume to ");
        DPRINTLN(volume);
        DFMiniMp3<SoftwareSerial, Mp3Notifier>::setVolume(volume);
        _curVolume = getVolume();
    } else {
        DPRINTF("Ignoring setVolume() because volume ");
        DPRINT(volume);
        DPRINTF(" is larger than ");
        DVPRINTLNF(_maxVolume);
    }
}

#endif /* !MP3PLAYER_H */
