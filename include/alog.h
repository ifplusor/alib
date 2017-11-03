/*
 * trace, debug, info, warn, error, fatal
 */
#ifndef _ALIB_LOG_H_
#define _ALIB_LOG_H_

#include <stdio.h>

typedef enum alog_level {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL
} alog_level;

#define ALOG_PREFIX(level, prefix) do { \
  switch (level) { \
  case FATAL: \
    prefix = "FATAL"; \
    break; \
  case ERROR: \
    prefix = "ERROR"; \
    break; \
  case WARN: \
    prefix = "WARN"; \
    break; \
  case INFO: \
    prefix = "INFO"; \
    break; \
  case DEBUG: \
    prefix = "DEBUG"; \
    break; \
  case TRACE: \
    prefix = "TRACE"; \
    break; \
  }; \
} while(0)

#define ALOG_FORMAT(level, stream, fmt, ...) do { \
  char *prefix; \
  ALOG_PREFIX(level, prefix); \
  fprintf(stream, "%s(%d)::%s ", __FILE__, __LINE__, __FUNCTION__); \
  fprintf(stream, "%s: ", prefix); \
  fprintf(stream, fmt, __VA_ARGS__); \
  fprintf(stream, "\n"); \
} while(0)

#define ALOG(level, msg) ALOG_FORMAT(level, stdout, "%s", msg)
#define ALOG_ERROR(msg) ALOG_FORMAT(ERROR, stderr, "%s", msg)

#endif // _ALIB_LOG_H_