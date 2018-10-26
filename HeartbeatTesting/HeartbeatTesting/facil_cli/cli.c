//
// Created by George Marr on 25/10/2018.
//

#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "../../packages/facil.io/lib/fio_cli.h"

void init_cli(int argc, const char *argv[]){
    fio_cli_start(argc, argv, 0, NULL,
            "-bind -b address to listen to. Default: Whatever is free",
            "-port -p port number to listen to. Defaults: 8080", FIO_CLI_TYPE_INT,
            "-workers -w number of process to use. Default: 1.", FIO_CLI_TYPE_INT,
            "-threads -t number of threads per process. Default: 1", FIO_CLI_TYPE_INT,
            "-log -v verbosity logging. Default: false", FIO_CLI_TYPE_BOOL,
            "-public -www folder for static file service. REQUIRED.",
            "-keep-alive -k HTTP keep-alive timeout (0 to 255). Default: 5s", FIO_CLI_TYPE_INT,
            "-ping websocket ping interval (0 to 255). Default: 40s",
            "-max-body -maxbd HTTP upload limit, Default: 50mb", FIO_CLI_TYPE_INT,
            "-max-message -maxmm incoming websocket message size limi. Default: 250KB", FIO_CLI_TPYE_INT,
            "-redis-url -ru Optional Redis server.");

    if(!fio_cli_get("-p")){
        char *tmp = getenv("PORT");
        if(!tmp) {
            fio_cli_set("-p", tmp);
            fio_cli_set("-port", tmp);
        }
    }

    if(!fio_cli_get("-b")){
        char *tmp = getenv("ADDRESS");
        if(tmp){
            fio_cli_set("-b", tmp);
            fio_cli_set("-bind", tmp);
        }
    }

    if(!fio_cli_get("-public")){
        char *tmp = getenv("HTTP_PUBLIC_FOLDER");
        if(tmp){
            fio_cli_set("-public", tmp);
            fio_cli_set("-www", tmp) ;
        }
    }

    if(!fio_cli_get("-public")){
        char *tmp = getenv("REDIS_URL");
        if(tmp){
            fio_cli_set("-redis-url", tmp);
            fio_cli_set("-ru", tmp);
        }
    }
}

void free_cli(void){
    fio_cli_end();
}