#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>

#ifdef ANDROID  // Android
#include <android/log.h>

#ifdef DEBUG
#define log_debug(...) \
        __android_log_print(ANDROID_LOG_DEBUG, "wst",  __VA_ARGS__)
#else
#define log_debug(...)
#endif

#define log_warn(...) \
        __android_log_print(ANDROID_LOG_WARN, "wst", __VA_ARGS__)
#define log_error(...) \
        __android_log_print(ANDROID_LOG_ERROR, "wst", __VA_ARGS__)
#define log_bug(...) \
        __android_log_print(ANDROID_LOG_FATAL, "wst", __VA_ARGS__)
#define log_info(...) \
        __android_log_print(ANDROID_LOG_INFO, "wst", __VA_ARGS__)

#else  // Linux

/*
 * This message means an internal bug. */
void log_bug(const char* format, ...);

/*
 * log the message. */
void log_info(const char* format, ...);

/*
 * This message means an error. */
void log_error(const char* format, ...);

/*
 * This message means that there is something unexpected 
 * that can be a potential problem. */
void log_warn(const char* format, ...);

/*
 * Just debug message. Disabled by default. */
#ifdef DEBUG
void log_debug(const char* format, ...);
#else
#define log_debug(...) 
#endif

#endif   // End of Linux or Android

#endif
