/*****************************************************************************
 * libvlcapi_api_media_player.c
 *****************************************************************************
 * Copyright (C) 2013 Rui Zhang
 *
 * Author: Rui Zhang <bbcallen@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <vlc/vlc.h>
#include <vlc_common.h>
#include <vlc_url.h>
#include <vlc_fourcc.h>

#include <jni.h>

#include "jni_helper.h"

#include "libvlcapi_misc.h"

#define LOG_TAG "libvlcapi-media-player"
#include "log.h"

typedef struct {
    jfieldID nativeInstance;
    jfieldID nativeMediaPlayer;
} media_player_fields_t;
static media_player_fields_t g_media_player_fields;

static bool g_initialized = false;
static void libVlcApiMediaPlayer_globalInit(JNIEnv *env) {
    if (g_initialized)
        return;

    jclass clazz;
    clazz = (*env)->FindClass(env, "tv/danmaku/media/vlc/LibVlcApiMediaPlayer");
    if (clazz == NULL) {
        return;
    }

    g_media_player_fields.nativeInstance = (*env)->GetFieldID(env, clazz,
            "mNativeInstance", "J");
    if (g_media_player_fields.nativeInstance == NULL) {
        return;
    }

    g_media_player_fields.nativeMediaPlayer = (*env)->GetFieldID(env, clazz,
            "mNativeMediaPlayer", "J");
    if (g_media_player_fields.nativeMediaPlayer == NULL) {
        return;
    }

    // more fields
    g_initialized = true;
    LOGV("libVlcApiMediaPlayer_globalInit completed");
}

static libvlc_instance_t *getNativeInstance(JNIEnv *env, jobject thiz) {
    return (libvlc_instance_t *) (intptr_t)(jlong)(*env)->GetLongField(env,
            thiz, g_media_player_fields.nativeInstance);
}

static void setNativeInstance(JNIEnv *env, jobject thiz,
        libvlc_instance_t *instance) {
    (*env)->SetLongField(env, thiz, g_media_player_fields.nativeInstance,
            (jlong) (intptr_t) instance);
}

static libvlc_media_player_t *getNativeMediaPlayer(JNIEnv *env, jobject thiz) {
    return (libvlc_media_player_t *) (intptr_t)(jlong)(*env)->GetLongField(env,
            thiz, g_media_player_fields.nativeMediaPlayer);
}

static void setNativeMediaPlayer(JNIEnv *env, jobject thiz,
        libvlc_media_player_t *instance) {
    (*env)->SetLongField(env, thiz, g_media_player_fields.nativeMediaPlayer,
            (jlong) (intptr_t) instance);
}

void Java_tv_danmaku_media_vlc_LibVlcApiMediaPlayer_nativeOpen(JNIEnv *env,
        jobject thiz, jobject weak_this) {
    LOGV("nativeOpen");

    libVlcApiMediaPlayer_globalInit(env);

    libvlc_instance_t *instance = getNativeInstance(env, thiz);

    /* Create a media player playing environment */
    libvlc_media_player_t *player = libvlc_media_player_new(instance);
    if (!instance) {
        libVlcApi_throwLibVlcException(env,
                "Unable to instantiate LibVlcApiMediaPlayer");
        return;
    }

    setNativeMediaPlayer(env, thiz, player);
}

void Java_tv_danmaku_media_vlc_LibVlcApiMediaPlayer_nativeClose(JNIEnv *env,
        jobject thiz) {
    LOGV("nativeClose");

    libvlc_media_player_t *player = getNativeMediaPlayer(env, thiz);
    if (player) {
        libvlc_media_player_stop(player);
        libvlc_media_player_release(player);
        setNativeMediaPlayer(env, thiz, NULL);
    }
}

