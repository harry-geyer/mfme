#pragma once

#include <sqlite3.h>

#include "util.h"


#define MFME_SQL_CREATE_TABLE_FILES                                     \
    "CREATE TABLE IF NOT EXISTS mfme_files ( "                          \
    "  id INTEGER PRIMARY KEY AUTOINCREMENT, "                          \
    "  name TEXT NOT NULL, "                                            \
    "  valid_since BIGINT NOT NULL, "                                   \
    "  valid_to BINGINT "                                               \
    ");"

#define MFME_SQL_CREATE_TABLE_META                                      \
    "CREATE TABLE IF NOT EXISTS mfme_meta ( "                           \
    "  id INTEGER PRIMARY KEY AUTOINCREMENT, "                          \
    "  name TEXT NOT NULL, "                                            \
    "  value TEXT, "                                                    \
    "  valid_since BIGINT NOT NULL, "                                   \
    "  valid_to BIGINT "                                                \
    ");"

#define MFME_SQL_INSERT_VALUE_META                                      \
    "INSERT INTO mfme_meta(name, value, valid_since) "                  \
    "VALUES (\"%s\", \"%s\", %s);"

#define MFME_SQL_CLOSE_VALUE_META                                       \
    "UPDATE mfme_meta SET valid_until = %s;"

#define MFME_SQL_QUERY_META                                             \
    "SELECT id, name, value, valid_since, valid_to"                     \
    "FROM mfme_meta "                                                   \
    "WHERE name = \"%s\" "                                              \
    "  AND valid_since <= %s "                                          \
    "  AND (valid_to IS NULL OR valid_to > %s);"


int mfme_sql_fmt(sqlite3* db, int (* cb)(void* userdata, int ncol, char** coltext, char** colres), void* userdata, char** errmsg, const char* fmt, ...) PRINTF_FMT_CHECK(5, 6);

