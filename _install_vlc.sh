#! /bin/sh

INSTALL_PROJECT=../android/VlcMediaPlayerDemo
cp -vR vlc-android/libs/* ${INSTALL_PROJECT}/libs/
rm ${INSTALL_PROJECT}/libs/armeabi/gdb.setup
rm ${INSTALL_PROJECT}/libs/armeabi/gdbserver
rm ${INSTALL_PROJECT}/libs/armeabi-v7a/gdb.setup
rm ${INSTALL_PROJECT}/libs/armeabi-v7a/gdbserver
