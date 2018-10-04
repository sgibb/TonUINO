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
        _pinBusy(pinBusy), _maxVolume(30) {};

    // overloaded
    void increaseVolume();
    void setVolume(uint8_t volume);

    bool isPlaying() { return !digitalRead(_pinBusy); };
    void setMaxVolume(uint8_t volume) { _maxVolume = volume; };

private:
    byte _pinBusy;
    uint8_t _maxVolume;
};

inline void Mp3Player::increaseVolume() {
    if (getVolume() < _maxVolume) {
        DPRINTLNF("Increase volume")
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
