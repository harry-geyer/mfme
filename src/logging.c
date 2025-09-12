#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#include "logging.h"


static int _mfme_logging_print_time(FILE* stream);


int mfme_logging_level = MFME_LOGGING_LEVEL_DEBUG;


int mfme_logging_out(FILE* stream, mfme_logging_level_t level, const char* fmt, ...)
{
    printf("mfme_logging_level = %d\n", mfme_logging_level);
    printf("level = %d\n", level);
    if (mfme_logging_level < level) {
        return 0;
    }
    int len = _mfme_logging_print_time(stream);
    va_list args;
    va_start(args);
    len += vfprintf(stream, fmt, args);
    len += fputc('\n', stream);
    va_end(args);
    return len;
}


static int _mfme_logging_print_time(FILE* stream)
{
    struct timeval tv;
    struct tm *tm_info;
    char t_buf[26];
    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);
    strftime(t_buf, sizeof(t_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return fprintf(stream, "%.*s.%06ld ", 25, t_buf, tv.tv_usec);
}

