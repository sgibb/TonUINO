#ifndef RFIDREADER_H
#define RFIDREADER_H

#include "debug.h"

#include <SPI.h>
#include <MFRC522.h>

struct RfidCard {
    uint32_t cookie;    // 4 Byte
    uint8_t version;    // 1 Byte
    uint8_t folder;     // 1 Byte
};

class RfidReader {
public:
    RfidReader(const byte pinChipSelect, const byte pinResetPowerDown)
        : _mfrc522(pinChipSelect, pinResetPowerDown) {};

    void begin();
    bool isNewRfidCardPresent();
    bool readCard(RfidCard *card);
    bool writeCard(RfidCard *card);
    void closeCard();

private:
    void dumpByteArray(byte *buffer, byte bufferSize);

    MFRC522 _mfrc522;
    MFRC522::MIFARE_Key _key;

    // MIFARE 1K: 16 sectors with 4 blocks,
    // sector 0, block 0 is manufactor block,
    // last block is (3) is always reserved for KeyA/AccessData/KeyB
    const byte _sector = 1;
    const byte _blockAddress = 4; // data: (4, 5, 6) * 16 Byte = 48 Byte
    const byte _trailingBlockAddress = 7;
};

void RfidReader::begin() {
    SPI.begin();
    _mfrc522.PCD_Init();
#ifdef DEBUG
    DPRINTF("RFID: ")
    _mfrc522.PCD_DumpVersionToSerial();
#endif // DEBUG

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    // Taken from: ../libraries/MFRC522/examples/ReadAndWrite/ReadAndWrite.ino
    for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
        _key.keyByte[i] = 0xFF;
    }
    DPRINTLNF("Key (for A and B):");
    dumpByteArray(_key.keyByte, MFRC522::MF_KEY_SIZE);
}

bool RfidReader::isNewRfidCardPresent() {
	return _mfrc522.PICC_IsNewCardPresent() && _mfrc522.PICC_ReadCardSerial();
}

bool RfidReader::readCard(RfidCard *card) {
#ifdef DEBUG
    DPRINTF("Card UID:");
    dumpByteArray(_mfrc522.uid.uidByte, _mfrc522.uid.size);
    DPRINTF("PICC type: ");
    MFRC522::PICC_Type piccType = _mfrc522.PICC_GetType(_mfrc522.uid.sak);
    DPRINT(_mfrc522.PICC_GetTypeName(piccType));
    DPRINTLN();
#endif // DEBUG

    // The buffer must be at least 18 bytes because a CRC_A is also returned.
    byte buffer[18];
    byte size = sizeof(buffer);

    // Authenticate using key A
    DPRINTLNF("Authenticating using key A ...");
    MFRC522::StatusCode status = _mfrc522.PCD_Authenticate(
            MFRC522::PICC_CMD_MF_AUTH_KEY_A,
            _trailingBlockAddress,
            &_key,
            &(_mfrc522.uid)
    );
    if (status != MFRC522::STATUS_OK) {
        DPRINTF("PCD_Authenticate() failed: ")
        DPRINTLN(_mfrc522.GetStatusCodeName(status));
        return false;
    }

    DPRINTF("Reading data from block ");
    DPRINT(_blockAddress);
    DPRINTLNF(" ...");
    status = _mfrc522.MIFARE_Read(_blockAddress, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        DPRINTF("MIFARE_Read() failed: ");
        DPRINTLN(_mfrc522.GetStatusCodeName(status));
        return false;
    }
    DPRINTF("Data in block ");
    DPRINT(_blockAddress);
    DPRINTF(":");
    dumpByteArray(buffer, 16);

    card->cookie =
        (static_cast<uint32_t>(buffer[0]) << 24) +
        (static_cast<uint32_t>(buffer[1]) << 16) +
        (static_cast<uint32_t>(buffer[2]) << 8) +
        static_cast<uint32_t>(buffer[3]);
    card->version = buffer[4];
    card->folder = buffer[5];

    return true;
}

bool RfidReader::writeCard(RfidCard *card) {
    byte buffer[] = {
        0x70, 0x11, 0x11, 0x10, // TonUNIO => leet speak: 70.11.11.10
                                // without "U" to identify our RFID tags
        0x01,                   // version 1
        card->folder,           // the folder picked by the user
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    byte size = sizeof(buffer);

    DPRINTLNF("Authenticating using key A ...");
    MFRC522::StatusCode status = _mfrc522.PCD_Authenticate(
            MFRC522::PICC_CMD_MF_AUTH_KEY_A,
            _trailingBlockAddress,
            &_key,
            &(_mfrc522.uid)
    );
    if (status != MFRC522::STATUS_OK) {
        DPRINTF("PCD_Authenticate() failed: ")
        DPRINTLN(_mfrc522.GetStatusCodeName(status));
        return false;
    }

    DPRINTF("Writing data into block ");
    DPRINT(_blockAddress);
    DPRINTLNF(" ...");
    status = _mfrc522.MIFARE_Write(_blockAddress, buffer, size);
    if (status != MFRC522::STATUS_OK) {
        DPRINTF("MIFARE_Write() failed: ");
        DPRINTLN(_mfrc522.GetStatusCodeName(status));
        return false;
    }
}

void RfidReader::closeCard() {
    DPRINTLNF("Close RFID-Card.");
    _mfrc522.PICC_HaltA();
    _mfrc522.PCD_StopCrypto1();
}

void RfidReader::dumpByteArray(byte *buffer, byte bufferSize) {
#ifdef DEBUG
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
    Serial.println();
    Serial.flush();
#endif // DEBUG
}

#endif /* !RFIDREADER_H */
