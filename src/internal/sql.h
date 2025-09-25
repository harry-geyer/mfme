#pragma once

#include <sqlite3.h>

#include "util.h"


#define MFME_SQL_CREATE_TABLE_META                                      \
    "CREATE TABLE IF NOT EXISTS mfme_meta ( \n"                         \
    "  id INTEGER PRIMARY KEY AUTOINCREMENT, \n"                        \
    "  name TEXT NOT NULL, \n"                                          \
    "  value TEXT, \n"                                                  \
    "  valid_since BIGINT NOT NULL, \n"                                 \
    "  valid_to BIGINT \n"                                              \
    ");"

#define MFME_SQL_CREATE_TABLE_FILESTORE_TYPES                           \
    "CREATE TABLE IF NOT EXISTS mfme_filestore_types ( \n"              \
    "  id INTEGER PRIMARY KEY AUTOINCREMENT, \n"                        \
    "  name TEXT NOT NULL \n"                                           \
    ");"


#define MFME_SQL_CREATE_TABLE_FILESTORES                                \
    "CREATE TABLE IF NOT EXISTS mfme_filestores ( \n"                   \
    "  id INTEGER PRIMARY KEY AUTOINCREMENT, \n"                        \
    "  type_id INT, \n"                                                 \
    "  name TEXT NOT NULL \n"                                           \
    "  location TEXT NOT NULL \n"                                       \
    "  FOREIGN KEY (type_id) \n"                                        \
    "    REFERENCES mfme_filestore_types (id) \n"                       \
    ");"

#define MFME_SQL_CREATE_TABLE_FILES                                     \
    "CREATE TABLE IF NOT EXISTS mfme_files ( \n"                        \
    "  id INTEGER PRIMARY KEY AUTOINCREMENT, \n"                        \
    "  name TEXT NOT NULL, \n"                                          \
    "  filestore_id INT NOT NULL, \n"                                   \
    "  valid_since BIGINT NOT NULL, \n"                                 \
    "  valid_to BINGINT, \n"                                            \
    "  FOREIGN KEY (filestore_id) \n"                                   \
    "    REFERENCES mfme_filestores (id) \n"                            \
    ");"

#define MFME_SQL_INSERT_VALUE_META                                      \
    "INSERT INTO mfme_meta(name, value, valid_since) \n"                \
    "VALUES (\"%s\", \"%s\", %s);"

#define MFME_SQL_CLOSE_VALUE_META                                       \
    "UPDATE mfme_meta SET valid_until = %s;"

#define MFME_SQL_QUERY_META                                             \
    "SELECT id, name, value, valid_since, valid_to \n"                  \
    "FROM mfme_meta \n"                                                 \
    "WHERE name = \"%s\" \n"                                            \
    "  AND valid_since <= %s \n"                                        \
    "  AND (valid_to IS NULL OR valid_to > %s);"

#define MFME_SQL_INSERT_FILESTORE_TYPE                                  \
    "INSERT INTO mfme_filestore_types(name) \n"                         \
    "VALUES (\"%s\");"

#define MFME_SQL_QUERY_FILESTORE_TYPE                                   \
    "SELECT id \n"                                                      \
    "FROM mfme_filestore_types \n"                                      \
    "WHERE name = \"%s\" \n"

#define MFME_SQL_INSERT_FILESTORE                                       \
    "INSERT INTO mfme_filestores(type_id, name, location) \n"           \
    "VALUES (\n "                                                       \
    "  (SELECT id FROM mfme_filestore_types WHERE name = \"%s\"), \n"   \
    "  \"%s\", \n"                                                      \
    "  \"%s\" \n"                                                       \
    ");"

#define MFME_SQL_QUERY_FILESTORE                                        \
    "SELECT \n"                                                         \
    "  mfme_filestore_types.name AS \"type\", \n"                       \
    "  mfme_filestores.location, \n"                                    \
    "FROM mfme_filestore_types, \n"                                     \
    "JOIN mfme_filestores \n"                                           \
    "ON mfme_filestores.type_id = mfme_filestore_types.id \n"           \
    "  AND mfme_filestores.name = \"%s\";"

#define MFME_SQL_INSERT_FILE                                            \
    "INSERT INTO mfme_files(name, filestore, valid_since) \n"           \
    "VALUES ( \n"                                                       \
    "  \"%s\", \n"                                                      \
    "  (SELECT id FROM mfme_filestores WHERE name = \"%s\"), \n"        \
    "  %s \n"                                                           \
    ");"

#define MFME_SQL_CLOSE_FILE                                             \
    "UPDATE mfme_files \n"                                              \
    "SET valid_to = %s \n"                                              \
    "WHERE name = \"%s\" \n"                                            \
    "  AND filestore_id = ( \n"                                         \
    "    SELECT id FROM mfme_filestores WHERE name = \"%s\" \n"         \
    "  );"

#define MFME_SQL_QUERY_FILE                                             \
    "SELECT \n"                                                         \
    "  mfme_files.id, \n"                                               \
    "  mfme_files.name, \n"                                             \
    "  mfme_filestores.location, \n"                                    \
    "  mfme_filestores.name AS \"filestore\", \n"                       \
    "  mfme_filestore_types.name AS \"filestore_type\", \n"             \
    "  mfme_files.valid_since, \n"                                      \
    "  mfme_files.valid_from \n"                                        \
    "FROM mfme_filestore_types \n"                                      \
    "JOIN mfme_filestores \n"                                           \
    "ON mfme_filestores.type_id = mfme_filestore_types.id \n"           \
    "  AND mfme_filestores.location = \"%s\" \n"                        \
    "JOIN mfme_files \n"                                                \
    "ON mfme_files.filestore_id = mfme_filestores.id \n"                \
    "  AND mfme_files.valid_since <= %s \n"                             \
    "  AND (mfme_files.valid_to IS NULL \n"                             \
    "    OR mfme_files.valid_to > %s) \n"                               \
    "  AND mfme_files.name = \"%s\" \n"


int mfme_sql_fmt(sqlite3* db, int (* cb)(void* userdata, int argc, char** argv, char** azcol_name), void* userdata, char** errmsg, const char* fmt, ...) PRINTF_FMT_CHECK(5, 6);

