#include <stdio.h>

#include <mongoose.h>


static void _handle_connection(struct mg_connection* c, int ev, void* ev_data);


int main(void)
{
    printf("Hello world\n");
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_log_set(MG_LL_DEBUG);
    mg_http_listen(&mgr, "127.0.0.1:8123", _handle_connection, NULL);
    while (1) {
        mg_mgr_poll(&mgr, 50);
    }
    mg_mgr_free(&mgr);
    return 0;
}


static void _handle_connection(struct mg_connection* c, int ev, void* ev_data)
{
}

