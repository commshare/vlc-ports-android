/*****************************************************************************
 * libvlcapi_log.c
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

#include <jni.h>

#define LOG_TAG "libvlcapi-log"
#include "log.h"

static void debug_log(void *data, int level, const char *fmt, va_list ap) {
    bool *verbose = data;

    static const uint8_t priority[5] = { [LIBVLC_DEBUG] = ANDROID_LOG_DEBUG, [1 /* ??? */
    ] = ANDROID_LOG_DEBUG, [LIBVLC_NOTICE] = ANDROID_LOG_INFO, [LIBVLC_WARNING
            ] = ANDROID_LOG_WARN, [LIBVLC_ERROR ] = ANDROID_LOG_ERROR, };

    int prio = ANDROID_LOG_DEBUG;
    if (level >= LIBVLC_DEBUG && level <= LIBVLC_ERROR)
        prio = priority[level];

    if (!*verbose && prio < ANDROID_LOG_ERROR)
        return;

    __android_log_vprint(prio, "VLC", fmt, ap);
}

static libvlc_log_subscriber_t g_debug_subscriber;
static bool g_verbosity;

static bool g_initialized = false;
static void libVlcLog_globalInit(JNIEnv *env, jboolean verbose) {
    if (g_initialized)
        return;

    g_verbosity = verbose;
    libvlc_log_subscribe(&g_debug_subscriber, debug_log, &g_verbosity);

    g_initialized = true;
    LOGV("libVlcLog_globalInit completed");
}

void Java_tv_danmaku_media_vlc_LibVlcApiLog_nativeOpen(JNIEnv *env,
        jobject thiz, jboolean verbose) {
    LOGV("nativeOpen");

    libVlcLog_globalInit(env, verbose);
}

void Java_tv_danmaku_media_vlc_LibVlcApiLog_nativeClose(JNIEnv *env,
        jobject thiz) {
    LOGV("nativeClose");

    if (g_initialized) {
        libvlc_log_unsubscribe(&g_debug_subscriber);
        g_initialized = false;
    }
}

void Java_tv_danmaku_media_vlc_LibVlcApiLog_nativeSetVerbosity(JNIEnv *env,
        jobject thiz, jboolean verbose) {
    g_verbosity = verbose;
    if (g_initialized) {
        libvlc_log_unsubscribe(&g_debug_subscriber);
    }
    libvlc_log_subscribe(&g_debug_subscriber, debug_log, &g_verbosity);
    g_initialized = true;
}
