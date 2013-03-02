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

#include <stddef.h>
#include <string.h>

/*-
 Type Signature             Java Type
 Z                          boolean
 B                          byte
 C                          char
 S                          short
 I                          int
 J                          long
 F                          float
 D                          double
 L fully-qualified-class ;  fully-qualified-class
 [ type                     type[]
 ( arg-types ) ret-type     method type
 */

// string array
typedef struct {
    jarray j_array;
    int size;

    jstring *jstring_array;
    char **utf_chars_array;
} jnihelper_string_array_t;

void jnihelper_string_array_detach(JNIEnv *env,
        jnihelper_string_array_t *p_string_array);

void jnihelper_string_array_attach(JNIEnv *env,
        jnihelper_string_array_t *p_string_array, jarray j_array);

inline static void jnihelper_string_array_reset_zero(
        jnihelper_string_array_t *p_string_array) {
    if (p_string_array) {
        memset(p_string_array, 0, sizeof(jnihelper_string_array_t));
    }
}

inline static int jnihelper_string_array_is_empty(
        jnihelper_string_array_t *p_string_array) {
    return !p_string_array || !p_string_array->j_array
            || p_string_array->size <= 0 || p_string_array->jstring_array
            || p_string_array->utf_chars_array;
}
