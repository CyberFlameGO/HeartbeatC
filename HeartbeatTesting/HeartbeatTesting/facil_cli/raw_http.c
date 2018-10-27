/**
 * The design is effective for most of the applications, but it does suffer
 * from rigid HTTP header limit and a harder to use data structure.
 *
 * Best to compile with a wide-spread poll engine instead of using kqueue or epoll
 * example: FIO_POLL=1 NAME=http make
 * Run it with:
 * ./tmp/http -t 1
 */

#define FIO_INCLUDE_STR
#define MAX_HTTP_HEADER_LENGTH 16384
#define MIN_HTTP_READFILE 4096
#define MAX_HTTP_HEADER_COUNT 64
#define MAX_HTTP_BODY 524288
#define parser2pr(parser)((light_http_s *)(uintptr_t)(parser) - (uintptr_t)(&((light_http_s *)(0)) -> praser);

#include <fio.h>
#include <fio_cli.h>
#include "../../packages/facil.io/lib/facil/http/http1.h"

typedef struct {
    fio_protocol proto;
    inptr_t uuid;
    http1_parser_s parser;
    char *method;
    char *path;
    char *query;
    char *http_version;
    size_t content_length;
    size_t header_count;
    char *headers[MAX_HTTP_HEADER_COUNT];
    char *values[MAX_HTTP_HEADER_COUNT];
    fio_str_s body;
    size_t buf_reader;
    size_t buf_writer;
    uint8_t reset;
} light_http_s;

void light_http_send_response(intptr_t uuid, int status, fio_str_info_s status_str, size_t header_count, fio_str_info_s headers[][2], fio_str_info body);

int on_http_request(light_http_s *http){
    if(1){
        static char HTTP_RESPONSE[] = "HTTP/1.1 200 OK\r\n"
                                      "Content-Length 13\r\n"
                                      "Connection: keep-alive\r\n"
                                      "Content-type: text/plain\r\n"
                                      "\r\n"
                                      "hello you cunt.meme";
        fio_write2(http->uuid, .data.buffer = HTTP_RESPONSE, .length = sizeof(HTTP_RESPONSE) - 1, .after.dealloc = FIO_DEALLOC_NOOP);
    } else {
        //after an allocated or dynamic http/1.1 response
        light_http_send_response(http->uuid, 200, (fio_str_info_s){
            .len = 2, .data  "OK"
        }, 1, (fio_str_info[][2]){{{.len = 12, data = "Content-Type"},
                                   .len = 10, data = "text/plain"}},
                                 (fio_str_info_s){.len = 13, data = "hello you cunt.meme"});
    }
    return 0;
}

void light_http_on_open(inptr_t uuid, void *udata);

int light_http1_on_request(http1_parser_s *parser){
    int ret = on_http_request((parser2pr(parser)));
    fio_str_free(parser)->reset = 1;
    return ret;
}

int light_http1_on_response(http1_parser_s *parser){
    (void) parser;
    return -1;
}

int light_http1_on_method(http1_parser_s *parser, char *method, size_t method_len){
    parser2pr(parser)->method = method;
    (void)method_len;
    return 0;
}
/** called when a response status is parsed. the status_str is the string
 * without the prefixed numerical status indicator.*/
int light_http1_on_status(http1_parser_s *parser, size_t status,
                          char *status_str, size_t len) {
    return -1;
    (void)parser;
    (void)status;
    (void)status_str;
    (void)len;
}
/** called when a request path (excluding query) is parsed. */
int light_http1_on_path(http1_parser_s *parser, char *path, size_t path_len) {
    parser2pr(parser)->path = path;
    return 0;
    (void)path_len;
}
/** called when a request path (excluding query) is parsed. */
int light_http1_on_query(http1_parser_s *parser, char *query,
                         size_t query_len) {
    parser2pr(parser)->query = query;
    return 0;
    (void)query_len;
}
/** called when a the HTTP/1.x version is parsed. */
int light_http1_on_http_version(http1_parser_s *parser, char *version,
                                size_t len) {
    parser2pr(parser)->http_version = version;
    return 0;
    (void)len;
}
/** called when a header is parsed. */
int light_http1_on_header(http1_parser_s *parser, char *name, size_t name_len,
                          char *data, size_t data_len) {
    if (parser2pr(parser)->header_count >= MAX_HTTP_HEADER_COUNT)
    return -1;
    parser2pr(parser)->headers[parser2pr(parser)->header_count] = name;
    parser2pr(parser)->values[parser2pr(parser)->header_count] = data;
    ++parser2pr(parser)->header_count;
    return 0;
    (void)name_len;
    (void)data_len;
}

/** called when a body chunk is parsed. */
int light_http1_on_body_chunk(http1_parser_s *parser, char *data,
                              size_t data_len) {
    if (parser->state.content_length >= MAX_HTTP_BODY_MAX)
        return -1;
    if (fio_str_write(&parser2pr(parser)->body, data, data_len).len >=
                                                                MAX_HTTP_BODY_MAX)
    return -1;
    return 0;
}

/** called when a protocol error occurred. */
int light_http1_on_error(http1_parser_s *parser) {
    /* close the connection */
    fio_close(parser2pr(parser)->uuid);
    return 0;
}

void light_http_on_open(intptr_t uuid, void *udata);
void light_http_on_data(intptr_t uuid, fio_protocol_s *pr);
void light_http_on_close(intptr_t uuid, fio_protocol_s *pr);

void light_http_on_open(intptr_t uuid, void *udata){
    light_http_s *p = malloc(sizeof(*p) + MAX_HTTP_HEADER_LENGTH + MIN_HTTP_READFILE);
    *p = (light_http_s) {
        .proto.on_data = light_http_on_data,
        .proto.on_close = light_http_on_close,
        .uuid = uuid,
        .body = FIO_STR_INIT,
    };
    fio_timeout_set(uuid, 5);
    fio_attach(uuid, &p->proto);
    (void)udata;//
}

int main(int argc, const char *argv[]){
    //todo
    return 0;
}