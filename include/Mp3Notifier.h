#ifndef MP3NOTIFIER_H
#define MP3NOTIFIER_H

#include "debug.h"

class Mp3Notifier {
public:
    static void OnError(uint16_t errorCode) {
        switch(errorCode) {
            case DfMp3_Error_Busy: DPRINTLNF("busy)"); break;
            case DfMp3_Error_Sleeping: DPRINTLNF("sleeping)"); break;
            case DfMp3_Error_SerialWrongStack: DPRINTLNF("wrong stack)"); break;
            case DfMp3_Error_CheckSumNotMatch: DPRINTLNF("checksum not match)"); break;
            case DfMp3_Error_FileIndexOut: DPRINTLNF("file index out of range)"); break;
            case DfMp3_Error_FileMismatch: DPRINTLNF("file mismatch)"); break;
            case DfMp3_Error_Advertise: DPRINTLNF("advertise)"); break;
            default: DPRINTLNF("unknown)");
        }
    }
    static void OnPlayFinished(uint16_t track) {
        DPRINTF("Play finished for #");
        DPRINTLN(track);
    }
    static void OnCardOnline(uint16_t code) {
        DPRINTF("SD-Card online: ");
        DPRINTLN(code);
    }
    static void OnCardInserted(uint16_t code) {
        DPRINTF("SD-Card inserted: ");
        DPRINTLN(code);
    }
    static void OnCardRemoved(uint16_t code) {
        DPRINTF("SD-Card removed: ");
        DPRINTLN(code);
    }
};

#endif /* !MP3NOTIFIER_H */
