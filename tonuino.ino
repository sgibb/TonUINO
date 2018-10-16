#include <AceButton.h>

using namespace ace_button;

#define DEBUG
#include "include/debug.h"

#include "include/Counter.h"
#include "include/Mp3Player.h"
#include "include/RfidReader.h"

#define LONGPRESS 1000 // msec
#define CLICKDELAY 2500 // msec
#define RFIDMODECLICKS 5

AceButton buttonStartPause;
AceButton buttonVolUp;
AceButton buttonVolDown;
Counter counter(RFIDMODECLICKS, CLICKDELAY);

enum ButtonId {
    ButtonStart = 0,
    ButtonVolUp,
    ButtonVolDown
};

// Are we playing mp3 files or rewrite RFID tags?
bool Mp3Notifier::isModeMp3Player = true;
bool Mp3Notifier::isTrackFinished = false;
SoftwareSerial serial(2, 3);    // RX, TX
Mp3Player mp3(serial, 4);       // BUSY
RfidReader rfid(10, 9);         // CHIP SELECT, RESET

void checkButtons() {
    buttonStartPause.check();
    buttonVolUp.check();
    buttonVolDown.check();
}

void enableButtonFeatures() {
    ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
    buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
    buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
    buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterRepeatPress);
}

void disableButtonFeatures() {
    ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
    buttonConfig->clearFeature(ButtonConfig::kFeatureLongPress);
    buttonConfig->clearFeature(ButtonConfig::kFeatureRepeatPress);
}

void handleRfidCardButtons(AceButton* button, uint8_t eventType, uint8_t /* buttonState */);

void handleMp3PlayerButtons(AceButton* button, uint8_t eventType, uint8_t /* buttonState */) {
    switch(button->getId()) {
        case ButtonStart:
            switch(eventType) {
                case AceButton::kEventReleased:
                    DPRINTLNF("Button: Start/Pause")
                    mp3.togglePlayPause();
                    counter.trigger();
                    if (counter.isFinished()) {
                        counter.reset();
                        DPRINTLNF("Button: Start/Pause - switch to RFID handler")
                        Mp3Notifier::isModeMp3Player = false;
                        mp3.stop();
                        mp3.setFolder(1);
                        mp3.playMp3FolderTrack(Mp3MessageApplyNewRfidCard);
                        // use delay to avoid accidential switch to MP3 mode
                        delay(10000);
                        disableButtonFeatures();
                        ButtonConfig* buttonConfig =
                            ButtonConfig::getSystemButtonConfig();
                        buttonConfig->setEventHandler(handleRfidCardButtons);
                    }
                    break;
            }
            break;
        case ButtonVolUp:
            switch(eventType) {
                case AceButton::kEventReleased:
                    DPRINTLNF("Button: Next track")
                    mp3.nextTrack();
                    break;
                case AceButton::kEventRepeatPressed:
                case AceButton::kEventLongPressed:
                    DPRINTLNF("Button: Volume up")
                    mp3.increaseVolume();
                    break;
            }
            break;
        case ButtonVolDown:
            switch(eventType) {
                case AceButton::kEventReleased:
                    DPRINTLNF("Button: Previous track")
                    mp3.prevTrack();
                    break;
                case AceButton::kEventRepeatPressed:
                case AceButton::kEventLongPressed:
                    DPRINTLNF("Button: Volume Down")
                    mp3.decreaseVolume();
                    break;
            }
            break;
    }
}

void handleRfidCardButtons(AceButton* button, uint8_t eventType, uint8_t /* buttonState */) {
    switch(button->getId()) {
        case ButtonStart:
            switch(eventType) {
                case AceButton::kEventReleased:
                    DPRINTLNF("Button: Start/Pause - switch to MP3 handler")
                    Mp3Notifier::isModeMp3Player = true;
                    mp3.start();
                    mp3.setTrack(0);
                    mp3.nextTrack();
                    mp3.playAdvertisement(Mp3AdvertPlayModus);
                    enableButtonFeatures();
                    ButtonConfig* buttonConfig =
                        ButtonConfig::getSystemButtonConfig();
                    buttonConfig->setEventHandler(handleMp3PlayerButtons);
                    break;
            }
            break;
        case ButtonVolUp:
            switch(eventType) {
                case AceButton::kEventReleased:
                    DPRINTLNF("Button: Next folder")
                    mp3.nextFolder();
                    mp3.playMp3FolderTrack(mp3.folder());
                    break;
            }
            break;
        case ButtonVolDown:
            switch(eventType) {
                case AceButton::kEventReleased:
                    DPRINTLNF("Button: Previous folder")
                    mp3.prevFolder();
                    mp3.playMp3FolderTrack(mp3.folder());
                    break;
            }
            break;
    }
}

void setup() {
    SERIALBEGIN(115200);
    DPRINTLNF("SETUP");

    pinMode(A0, INPUT_PULLUP);
    buttonStartPause.init(A0, HIGH, ButtonStart);
    pinMode(A1, INPUT_PULLUP);
    buttonVolUp.init(A1, HIGH, ButtonVolUp);
    pinMode(A2, INPUT_PULLUP);
    buttonVolDown.init(A2, HIGH, ButtonVolDown);

    ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
    buttonConfig->setEventHandler(handleMp3PlayerButtons);
    enableButtonFeatures();

    counter.reset();

    mp3.begin();
    mp3.setMaxVolume(MAXALLOWEDVOLUME);
    mp3.setVolume(INITIALVOLUME);
    DVPRINTLNF(mp3.getVolume());
    DVPRINTLNF(mp3.getPlaybackMode());
    DVPRINTLNF(mp3.getTotalTrackCount());
    DVPRINTLNF(mp3.getTotalFolderCount());

    rfid.begin();
}

void loop() {
    DPRINTF("LOOP - ");
    DPRINTLN(millis());

#ifdef DEBUG
    if (Mp3Notifier::isModeMp3Player) {
        DVPRINTLNF(mp3.isBusy());
        DVPRINTLNF(mp3.getStatus());
        DVPRINTLNF(mp3.getVolume());
        DVPRINTLNF(mp3.folder());
        DVPRINTLNF(mp3.track());
    }
#endif

    mp3.loop();

    checkButtons();

    if (rfid.isNewRfidCardPresent()) {
        RfidCard card;
        DPRINTLNF("New RFID-Card found.");

        if (Mp3Notifier::isModeMp3Player) {
            if (rfid.readCard(&card)) {
                mp3.setFolder(card.folder);
                DVPRINTLNF(mp3.tracks());
                mp3.setTrack(0);
                mp3.nextTrack();
            } else {
                mp3.playAdvertisement(Mp3AdvertRfidReadFailed);
            }
        } else {
            card.folder = mp3.folder();
            if (rfid.writeCard(&card)) {
                mp3.playMp3FolderTrack(Mp3MessageNewRfidCardWritten);
            } else {
                mp3.playMp3FolderTrack(Mp3MessageNewRfidCardFailed);
            }
        }
        rfid.closeCard();
    }
}
