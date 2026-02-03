#ifndef BTH_H
#define BTH_H

#include <stdint.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif

#undef ENABLED_BUFFER_MAX

#define BYTES_PER_LINE 16

#ifndef VERSION
#define VERSION "1.2"
#endif

#ifdef _WIN32
#define fprintf_t fprintf_s
#define LOGI(...) fprintf_t(stderr, __VA_ARGS__)
#elifdef __ANDROID__
#define fprintf_t fprintf
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "bth", ##__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "bth", ##__VA_ARGS__)
#else
#define fprintf_t fprintf
#define LOGI(...) fprintf_t(stderr, __VA_ARGS__), fputc('\n', stderr)
#endif

#ifndef __ANDROID__
#define LOGE(...) LOGI("E: " __VA_ARGS__)
#endif

#define FREE(x) do { if ((x) != NULL) { free(x); (x) = NULL; } } while(0)
#define CLOSE_FILE(x) do { if ((x) != NULL) { fclose(x); (x) = NULL; } } while(0)

#ifdef ENABLED_BUFFER_MAX
#define BUFFER_MAX 4096
#define TEXT_MAX "4096"
#else
#define TEXT_MAX "No"
#endif

#endif // BTH_H
