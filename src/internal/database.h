#pragma once

#include <sqlite3.h>


sqlite3* mfme_database_init(const char* path);
void mfme_database_close(sqlite3* db);

