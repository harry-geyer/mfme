#include <stdio.h>

#include <mongoose.h>

#include "database.h"
#include "logging.h"


#define WEBROOT_DIR             "webroot"
#define UPLOAD_URI              "/upload"
#define SQLITE_DB               "files.db"


static void _mongoose_init(struct mg_mgr* mgr);
static void _handle_connection(struct mg_connection* c, int ev, void* ev_data);


int main(void)
{
    sqlite3* db = mfme_database_init(SQLITE_DB);
    if (!db) {
        return 1;
    }
    struct mg_mgr mgr;
    _mongoose_init(&mgr);
    while (1) {
        mg_mgr_poll(&mgr, 50);
    }
    mg_mgr_free(&mgr);
    mfme_database_close(db);
    return 0;
}


static void _mongoose_init(struct mg_mgr* mgr)
{
    mg_mgr_init(mgr);
    mg_log_set(MG_LL_DEBUG);
    const char* address = "127.0.0.1:8123";
    mfme_logging_info("Listening to %s", address);
    mg_http_listen(mgr, address, _handle_connection, NULL);
}


static void _handle_connection(struct mg_connection* c, int ev, void* ev_data)
{
    if (MG_EV_HTTP_MSG == ev) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
        if (mg_match(hm->uri, mg_str(UPLOAD_URI), NULL)) {
            mg_http_upload(c, hm, &mg_fs_posix, "/tmp", 99999);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = WEBROOT_DIR };
            mg_http_serve_dir(c, ev_data, &opts);
        }
    }
}

