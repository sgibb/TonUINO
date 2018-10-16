#!/bin/bash
ESPEAK=/usr/bin/espeak
VOICE="mb-de2"
ESPEAKARG="-v${VOICE} --stdout"
LAME=/usr/bin/lame
LAMEARG="-m m -"
SDDIR="sdcard"
MSGFILE="audio-messages.txt"

function speak {
    FILE=$(echo "${1}" | cut -d ":" -f 1).mp3
    MSG=$(echo "${1}" | cut -d ":" -f 2)
    echo "${MSG}" | \
    ${ESPEAK} ${ESPEAKARG} | \
    ${LAME} ${LAMEARG} ${SDDIR}/${FILE}
}

rm -rf ${SDDIR}/{advert,mp3}/*.mp3

for i in {1..255}; do
    speak "mp3/$(printf '%04d' ${i}):Ordner ${i}"
done

while read LINE; do
    if [ -n "${LINE}" ] && [ ${LINE:0:1} != '#' ] ; then
        speak "${LINE}"
    fi
done < ${MSGFILE}
