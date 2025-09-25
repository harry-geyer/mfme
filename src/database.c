#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "sql.h"
#include "logging.h"


#define _MFME_DATABASE_VERSION          "1.0.0"


static int _mfme_database_get_time(char* buf, unsigned size);
static int _mfme_database_version_cb(void* data, int argc, char** argv, char** az_col_name);


typedef struct _mfme_database_cb_ctx {
    sqlite3* db;
    char* time;
} _mfme_database_cb_ctx_t;


sqlite3* mfme_database_init(const char* path)
{
    sqlite3* db = NULL;
    int ret = sqlite3_open(path, &db);
    if (ret) {
        mfme_logging_critical("Failed opening database\n");
        return NULL;
    }
    char* error_msg = NULL;
    ret = mfme_sql_fmt(db, NULL, 0, &error_msg, MFME_SQL_CREATE_TABLE_META);
    if (ret) {
        mfme_logging_critical("Error creating meta table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return NULL;
    }
    char time[21]; // biggest uint64_t is 20 digits
    ret = _mfme_database_get_time(time, 21);
    if (0 > ret) {
        mfme_logging_critical("Error getting time\n");
        return NULL;
    }
    static _mfme_database_cb_ctx_t ctx;
    ctx.db = db;
    ctx.time = time;
    ret = mfme_sql_fmt(db, _mfme_database_version_cb, (void*)&ctx, &error_msg, MFME_SQL_QUERY_META, "version", time, time);
    if (ret) {
        mfme_logging_critical("Error querying meta table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return NULL;
    }
    return db;
}


void mfme_database_deinit(sqlite3* db)
{
    sqlite3_close(db);
}


static int _mfme_database_get_time(char* buf, unsigned size)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        return -1;
    }
    uint64_t time_ns = (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
    unsigned len = snprintf(buf, size, "%"PRIu64, time_ns);
    buf[size - 1] = 0;
    return len;
}


static int _mfme_database_version_cb(void* userdata, int argc, char** argv, char** az_col_name)
{
    bool found_name = false;
    bool found_value = false;
    for (int i = 0; i < argc; i++) {
        if (!found_name
            && 0 == strncmp("name", az_col_name[i], strlen("name"))
            && 0 == strncmp("version", argv[i], strlen("version"))) {
            found_name = true;
        }
        if (!found_value
            && 0 == strncmp("value", az_col_name[i], strlen("value"))
            && 0 == strncmp(_MFME_DATABASE_VERSION, argv[i], strlen(_MFME_DATABASE_VERSION))) {
            found_value = true;
        }
        if (found_name && found_value) {
            mfme_logging_info("Database up to date");
            return 0;
        }
    }
    if (found_name && !found_value) {
        mfme_logging_info("Database outdated");
        return -1;
    }
    mfme_logging_info("New database");
    _mfme_database_cb_ctx_t* ctx = (_mfme_database_cb_ctx_t*)userdata;
    char* error_msg = NULL;
    int ret = mfme_sql_fmt(ctx->db, NULL, 0, &error_msg, MFME_SQL_INSERT_VALUE_META, "version", _MFME_DATABASE_VERSION, ctx->time);
    if (ret) {
        mfme_logging_critical("Error inserting into meta table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return ret;
    }

    ret = mfme_sql_fmt(ctx->db, NULL, 0, &error_msg, MFME_SQL_CREATE_TABLE_FILESTORE_TYPES);
    if (ret) {
        mfme_logging_critical("Error creating filestore types table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return ret;
    }
    ret = mfme_sql_fmt(ctx->db, NULL, 0, &error_msg, MFME_SQL_CREATE_TABLE_FILESTORES);
    if (ret) {
        mfme_logging_critical("Error creating filestores table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return ret;
    }
    ret = mfme_sql_fmt(ctx->db, NULL, 0, &error_msg, MFME_SQL_CREATE_TABLE_FILES);
    if (ret) {
        mfme_logging_critical("Error creating files table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return ret;
    }

    /* Add default filestore type and filestore */
    ret = mfme_sql_fmt(ctx->db, NULL, 0, &error_msg, MFME_SQL_INSERT_FILESTORE_TYPE, "directory");
    if (ret) {
        mfme_logging_critical("Error inserting filestore type \"directory\": %s\n", error_msg);
        sqlite3_free(error_msg);
        return ret;
    }
    ret = mfme_sql_fmt(ctx->db, NULL, 0, &error_msg, MFME_SQL_INSERT_FILESTORE, "directory", "default", "./db_files");
    if (ret) {
        mfme_logging_critical("Error inserting default filestore: %s\n", error_msg);
        sqlite3_free(error_msg);
        return ret;
    }
    return 0;
}

