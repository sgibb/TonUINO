#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <SoftwareSerial.h>

#include "Mp3Notifier.h"

class Mp3Player : public DFMiniMp3<SoftwareSerial, Mp3Notifier> {
public:
    Mp3Player(SoftwareSerial& softwareSerial, byte pinBusy)
    : DFMiniMp3<SoftwareSerial, Mp3Notifier>(softwareSerial), mPinBusy(pinBusy) {};

    bool isPlaying() { return !digitalRead(mPinBusy); };

private:
    byte mPinBusy;
};

//template<class T_SERIAL_METHOD>
//bool Mp3Player<T_SERIAL_METHOD>::isPlaying() {
//    return !digitalRead(mPinBusy);
//}

#endif /* !MP3PLAYER_H */
