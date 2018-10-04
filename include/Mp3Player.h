#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>

#include "Mp3Notifier.h"

class Mp3Player : public DFMiniMp3<SoftwareSerial, Mp3Notifier> {
public:
    Mp3Player(SoftwareSerial& softwareSerial, byte pinBusy)
    : DFMiniMp3<SoftwareSerial, Mp3Notifier>(softwareSerial),
        _pinBusy(pinBusy), _maxVolume(30), _folder(1), _track(1) {};

    bool isPlaying() { return !digitalRead(_pinBusy); };
    void setMaxVolume(uint8_t volume) { _maxVolume = volume; };

    uint8_t folder() { return _folder; };
    void setFolder(uint8_t folder) { _folder = folder; }
    uint8_t track() { return _track; };
    void setTrack(uint8_t track) { _track = track; };
    uint16_t tracks() { return DFMiniMp3<SoftwareSerial, Mp3Notifier>::getFolderTrackCount(_folder); };

    void nextTrack();
    void prevTrack();

    // overloaded
    void increaseVolume();
    void setVolume(uint8_t volume);

private:
    byte _pinBusy;
    uint8_t _maxVolume;
    uint8_t _folder;
    uint8_t _track;
};

void Mp3Player::nextTrack() {
    DPRINTLNF("nextTrack()");
    _track = (tracks() > _track) ? ++_track : 1;
    DVPRINTLNF(_track);
    DFMiniMp3<SoftwareSerial, Mp3Notifier>::playFolderTrack(_folder, _track);
}

void Mp3Player::prevTrack() {
    DPRINTLNF("prevTrack()");
    _track = (_track > 1) ? --_track : 1;
    DVPRINTLNF(_track);
    DFMiniMp3<SoftwareSerial, Mp3Notifier>::playFolderTrack(_folder, _track);
}

inline void Mp3Player::increaseVolume() {
    if (getVolume() < _maxVolume) {
        DPRINTLNF("Increase volume");
        DFMiniMp3<SoftwareSerial, Mp3Notifier>::increaseVolume();
    } else {
        DPRINTF("Ignoring increaseVolume() because volume would be larger than ");
        DVPRINTLNF(_maxVolume);
    }
}

inline void Mp3Player::setVolume(uint8_t volume) {
    if (volume <= _maxVolume) {
        DPRINTF("Set volume to ");
        DPRINTLN(volume);
        DFMiniMp3<SoftwareSerial, Mp3Notifier>::setVolume(volume);
    } else {
        DPRINTF("Ignoring setVolume() because volume ");
        DPRINT(volume);
        DPRINTF("is larger than ");
        DVPRINTLNF(_maxVolume);
    }
}

#endif /* !MP3PLAYER_H */
