/**
 * alog.h - log utils
 *
 * @author James Yin <ywhjames@hotmail.com>
 *
 * Note:
 *   alog provide 6 level: trace, debug, info, warn, error, fatal
 */
#ifndef __ALIB_LOG_H__
#define __ALIB_LOG_H__

#include <stdio.h>

typedef enum _alog_level_ { trace, debug, info, warn, error, fatal } alog_level_e;

#define ALOG_PREFIX(level, prefix) \
  do {                             \
    switch (level) {               \
      case fatal:                  \
        (prefix) = "FATAL";        \
        break;                     \
      case error:                  \
        (prefix) = "ERROR";        \
        break;                     \
      case warn:                   \
        (prefix) = "WARN";         \
        break;                     \
      case info:                   \
        (prefix) = "INFO";         \
        break;                     \
      case debug:                  \
        (prefix) = "DEBUG";        \
        break;                     \
      case trace:                  \
        (prefix) = "TRACE";        \
        break;                     \
    };                             \
  } while (0)

#if ALIB_DEBUG
#define ALOG_FORMAT(level, stream, fmt, ...)                          \
  do {                                                                \
    char* prefix;                                                     \
    ALOG_PREFIX(level, prefix);                                       \
    fprintf(stream, "%s(%d)::%s ", __FILE__, __LINE__, __FUNCTION__); \
    fprintf(stream, "%s: ", prefix);                                  \
    fprintf(stream, fmt, __VA_ARGS__);                                \
    fprintf(stream, "\n");                                            \
  } while (0)
#else
#define ALOG_FORMAT(level, stream, fmt, ...)                              \
  do {                                                                    \
    char* prefix;                                                         \
    ALOG_PREFIX(level, prefix);                                           \
    size_t len = strlen(fmt) + 14;                                        \
    char* format = malloc(len);                                           \
    snprintf(format, len, "%%s(%%d) %%s: %s\\n", fmt);                    \
    fprintf(stream, format, __FUNCTION__, __LINE__, prefix, __VA_ARGS__); \
    free(format);                                                         \
  } while (0)
#endif

/**
 * output to stdout
 */
#define ALOG(level, msg) ALOG_FORMAT(level, stdout, "%s", msg)

/**
 * output to stderr
 */
#define ALOG_ERROR(msg) ALOG_FORMAT(error, stderr, "%s", msg)

/**
 * output to stderr, than exit.
 */
#define ALOG_FATAL(msg)                    \
  do {                                     \
    ALOG_FORMAT(fatal, stderr, "%s", msg); \
    exit(-1);                              \
  } while (0)

#endif  // __ALIB_LOG_H__
