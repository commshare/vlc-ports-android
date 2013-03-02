/*****************************************************************************
 * jni_helper.c
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

#include <stdlib.h>

#include <jni.h>

#include "jni_helper.h"

#define LOG_TAG "jni_helper"
#include "log.h"

void jnihelper_string_array_attach(JNIEnv *env,
        jnihelper_string_array_t *p_string_array, jarray j_array) {
    if (!j_array)
        return;

    jnihelper_string_array_reset_zero(p_string_array);
    int size = (*env)->GetArrayLength(env, j_array);
    if (size <= 0)
        return;

    jstring *jstring_array = (jstring *) calloc(size, sizeof(jstring));
    if (!jstring_array) {
        // FIXME: throw exception
        return;
    }

    char **utf_chars_array = (char **) calloc(size, sizeof(char *));
    if (!utf_chars_array) {
        // FIXME: throw exception
        free(p_string_array);
        return;
    }

    for (int i = 0; i < size; i++) {
        jstring_array[i] = (*env)->GetObjectArrayElement(env, j_array, i);
        if (!jstring_array[i])
            continue;

        utf_chars_array[i] = (char *) (*env)->GetStringUTFChars(env,
                jstring_array[i], NULL);
    }

    p_string_array->j_array = j_array;
    p_string_array->size = size;
    p_string_array->jstring_array = jstring_array;
    p_string_array->utf_chars_array = utf_chars_array;
}

void jnihelper_string_array_detach(JNIEnv *env,
        jnihelper_string_array_t *p_string_array) {
    p_string_array->j_array = 0;
    p_string_array->size = 0;

    if (p_string_array->utf_chars_array && p_string_array->jstring_array) {
        size_t size = p_string_array->size;
        jstring * jstring_array = p_string_array->jstring_array;
        char **utf_chars_array = p_string_array->utf_chars_array;

        for (int i = 0; i < size; i++) {
            if (jstring_array[i] && utf_chars_array[i]) {
                (*env)->ReleaseStringUTFChars(env, jstring_array[i],
                        utf_chars_array[i]);
                jstring_array[i] = NULL;
                utf_chars_array[i] = NULL;
            }
        }
    }

    if (p_string_array->utf_chars_array) {
        free(p_string_array->utf_chars_array);
        p_string_array->utf_chars_array = NULL;
    }

    if (p_string_array->jstring_array) {
        free(p_string_array->jstring_array);
        p_string_array->jstring_array = NULL;
    }
}
