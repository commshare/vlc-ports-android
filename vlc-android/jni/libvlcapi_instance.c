/*****************************************************************************
 * libvlcapi_instance.c
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

#define LOG_TAG "libvlcapi-instance"
#include "log.h"

typedef struct {
    jfieldID nativeInstance;
} instance_fields_t;
static instance_fields_t g_instance_fields;

static bool g_initialized = false;
static void libVlcApiInstance_globalInit(JNIEnv *env) {
    if (g_initialized)
        return;

    jclass clazz;
    clazz = (*env)->FindClass(env, "tv/danmaku/media/vlc/LibVlcApiInstance");
    if (clazz == NULL) {
        return;
    }

    g_instance_fields.nativeInstance = (*env)->GetFieldID(env, clazz,
            "mNativeInstance", "J");
    if (g_instance_fields.nativeInstance == NULL) {
        return;
    }

    // more fields

    g_initialized = true;
    LOGV("libVlcApiInstance_globalInit completed");
}

static libvlc_instance_t *getNativeInstance(JNIEnv *env, jobject thiz) {
    return (libvlc_instance_t *) (intptr_t)(jlong)(*env)->GetLongField(env,
            thiz, g_instance_fields.nativeInstance);
}

static void setNativeInstance(JNIEnv *env, jobject thiz,
        libvlc_instance_t *instance) {
    (*env)->SetLongField(env, thiz, g_instance_fields.nativeInstance,
            (jlong) (intptr_t) instance);
}

void Java_tv_danmaku_media_vlc_LibVlcApiInstance_nativeOpen(JNIEnv *env,
        jobject thiz, jobject weak_this, jarray arguments) {
    LOGV("nativeOpen");

    libVlcApiInstance_globalInit(env);

    jnihelper_string_array_t arg_array;
    jnihelper_string_array_attach(env, &arg_array, arguments);

    libvlc_instance_t *instance = libvlc_new(arg_array.size,
            (char const * const *) arg_array.utf_chars_array);

    jnihelper_string_array_detach(env, &arg_array);
    if (!instance) {
        libVlcApi_throwLibVlcException(env,
                "Unable to instantiate LibVlcApiInstance");
        return;
    }

    setNativeInstance(env, thiz, instance);
}

void Java_tv_danmaku_media_vlc_LibVlcApiInstance_nativeClose(JNIEnv *env,
        jobject thiz) {
    LOGV("nativeClose");

    libvlc_instance_t *instance = getNativeInstance(env, thiz);
    if (instance) {
        libvlc_release(instance);
        setNativeInstance(env, thiz, NULL);
    }
}
