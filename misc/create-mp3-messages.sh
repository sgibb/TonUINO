#!/bin/bash

ESPEAK=/usr/bin/espeak
VOICE="mb-de2"
ESPEAKARG="-v${VOICE} -s 150 --stdout"
LAME=/usr/bin/lame
LAMEARG="-m m -"
SDDIR="sdcard"

function speak {
    echo "${1}" | \
    ${ESPEAK} ${ESPEAKARG} | \
    ${LAME} ${LAMEARG} ${SDDIR}/${2}
}

## advert
speak "Leider konnte ich die Karte nicht richtig lesen." \
    "advert/0001-rfid-read-failed.mp3"

## mp3
speak "Bitte lege eine Karte auf, die neu beschrieben soll." \
    "mp3/0001-write-new-card.mp3"
