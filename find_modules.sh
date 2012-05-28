#!/bin/sh

if [ $# != 1 ]
then
    echo "Usage: $0 [vlc build dir]"
    exit 1
fi

blacklist="
stats
access_(bd|shm|imem)
oldrc
real
hotkeys
gestures
sap
dynamicoverlay
rss
ball
audiobargraph_[av]
clone
mosaic
osdmenu
puzzle
mediadirs
t140
ripple
motion
sharpen
grain
posterize
mirror
wall
scene
blendbench
psychedelic
alphamask
netsync
audioscrobbler
motiondetect
motionblur
export
smf
podcast
bluescreen
erase
stream_filter_record
speex_resampler
remoteosd
magnify
gradient
.*tospdif
dtstofloat32
visual
fb
aiff
aout_file
yuv
.dummy
access_(attachment|mms|rar|udp|vrd)
aout_file
asf
avi
ay
compressor
decomp
dirac
dts
flac
flacsys
folder
karaoke
marq
ogg
osd_parser
packetizer_(dirac|flac)
playlist
rtp
sdp
speex
stream_filter_(dash|rar)
subtitle
(cvd|svc|dvd)sub
subsde
subsdelay
tta
visual
vobsub
voc
wav
wave
xml
zip
"

regexp=
for i in ${blacklist}
do
    if [ -z "${regexp}" ]
    then
        regexp="${i}"
    else
        regexp="${regexp}|${i}"
    fi
done

find $1/modules -name 'lib*plugin.a' | grep -vE "lib(${regexp})_plugin.a" | tr '\n' ' '
