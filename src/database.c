#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#include "sql.h"


static int _mfme_database_get_time(char* buf, unsigned size);


sqlite3* mfme_database_init(const char* path)
{
    sqlite3* db = NULL;
    int ret = sqlite3_open(path, &db);
    if (ret) {
        fprintf(stderr, "Failed opening database\n");
        return NULL;
    }
    char* error_msg = NULL;
    ret = mfme_sql_fmt(db, NULL, 0, &error_msg, MFME_SQL_CREATE_TABLE_META);
    if (ret) {
        fprintf(stderr, "Error creating meta table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return NULL;
    }
    char time[21]; // biggest uint64_t is 20 digits
    ret = _mfme_database_get_time(time, 21);
    if (0 > ret) {
        fprintf(stderr, "Error getting time\n");
        return NULL;
    }
    ret = mfme_sql_fmt(db, NULL, 0, &error_msg, MFME_SQL_INSERT_VALUE_META, "version", "1.0.0", time);
    if (ret) {
        fprintf(stderr, "Error inserting into meta table: %s\n", error_msg);
        sqlite3_free(error_msg);
        return NULL;
    }
    return db;
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
