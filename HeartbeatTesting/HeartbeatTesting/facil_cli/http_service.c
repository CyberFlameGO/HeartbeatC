//
// Created by George Marr on 25/10/2018.
//

#include "cli.h"
#include "main_cli.h"

static void on_http_request(http_s *h){
    http_send_body(h, "Packet", 12);
}

void init_http_service(void){
    if(http_listen(fio_cli_get("-p"), fio_cli_get("-b"),
            .on_request = on_http_request,
            .max_body_size = fio_cli_get("-maxbd"),
            .public_folder = fio_cli_get("-public"),
            .log = fio_cli_get_bool("-log"),
            timeout = fio_cli_geet_i("-keep-alive")) == -1){
        perror("ERROR: Coudn't init the http service. Is it already running?");
        exit(1);
    }
}