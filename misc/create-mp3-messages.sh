#!/bin/bash
ESPEAK=/usr/bin/espeak
VOICE="mb-de2"
ESPEAKARG="-v${VOICE} -s 150 --stdout"
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

while read LINE; do
    if [ -n "${LINE}" -a ${LINE:0:1} != '#' ] ; then
        speak "${LINE}"
    fi
done < ${MSGFILE}
