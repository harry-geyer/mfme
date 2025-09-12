#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include "logging.h"


int mfme_sql_fmt(sqlite3* db, int (* cb)(void* userdata, int argc, char** argv, char** azcol_name), void* userdata, char** errmsg, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    if (0 > len) {
        va_end(args);
        return -1;
    }
    char* stmt = malloc(len+1);
    if (!stmt) {
        va_end(args);
        return -1;
    }
    va_start(args, fmt);
    len = vsnprintf(stmt, len, fmt, args);
    va_end(args);
    if (0 > len) {
        free(stmt);
        return -1;
    }
    stmt[len] = '\0';
    mfme_logging_debug("SQL:\n%s", stmt);
    int ret = sqlite3_exec(db, stmt, cb, userdata, errmsg);
    free(stmt);
    return ret;
}

