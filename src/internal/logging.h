#pragma once

#include "util.h"

#ifndef MFME_LOGGING_STREAM
#define MFME_LOGGING_STREAM         stderr
#endif


typedef enum mfme_logging_level {
    MFME_LOGGING_LEVEL_DEBUG = 0,
    MFME_LOGGING_LEVEL_INFO = 10,
    MFME_LOGGING_LEVEL_WARNING = 20,
    MFME_LOGGING_LEVEL_ERROR = 30,
    MFME_LOGGING_LEVEL_CRITICAL = 40,
} mfme_logging_level_t;


extern int mfme_logging_level;

int mfme_logging_out(FILE* stream, mfme_logging_level_t level, const char* fmt, ...) PRINTF_FMT_CHECK(3, 4);

#define mfme_logging_debug(...)         mfme_logging_out(stderr, MFME_LOGGING_LEVEL_DEBUG, "[DBUG] " __VA_ARGS__)
#define mfme_logging_info(...)          mfme_logging_out(stderr, MFME_LOGGING_LEVEL_INFO, "[DBUG] " __VA_ARGS__)
#define mfme_logging_warning(...)       mfme_logging_out(stderr, MFME_LOGGING_LEVEL_WARNING, "[WARN] " __VA_ARGS__)
#define mfme_logging_error(...)         mfme_logging_out(stderr, MFME_LOGGING_LEVEL_ERROR, "[ERRO] " __VA_ARGS__)
#define mfme_logging_critical(...)      mfme_logging_out(stderr, MFME_LOGGING_LEVEL_CRITICAL, "[CRIT] " __VA_ARGS__)

