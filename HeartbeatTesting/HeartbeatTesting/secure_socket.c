#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define FAIL -1

int open_connection(const char *hostname, int port) {
	int sd;
	struct hostent *host;
	struct sockaddr_in addr;

	if ((host = gethostbyname(hostname)) == NULL) {
		perror(hostname);
		abort();
	}

	sd = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long *)(host->h_addr);

	if (connect(sd, (struct socketaddr*) &addr, sizeof(addr)) != 0) {
		close(sd);
		perror(hostname);
		abort();
	}
	return sd;
}

SSL_CTX* init_ctx(void) {

	SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	method = TLSv1_2_client_method();
	ctx = SSL_CTX_new(method);

	if (ctx == NULL) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

void show_certs(SSL* ssl) {
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl);

	if (cert != NULL) {
		printf("Server certs: \n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n");
		free(line);
		printf("Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	}
	else {
		printf("No certs have been configured\n");
	}
}

int main(int argc, char *strings[]) {
	SSL_CTX *ctx;
	int server;
	SSL *ssl;
	char buf[1024];
	char acClientRequest[1024] = { 0 };
	int bytes;
	char *hostname, *port;

	if (argc != 3) {
		printf("Incorrect usage: %s <hostname> <port> \n", strings[0]);
		exit(0);
	}
	SSL_library_init();
	hostname = strings[1];
	port = strings[2];

	ctx = init_ctx();
	server = open_connection(hostname, atoi(port));
	SSL_set_fd(ssl, server);

	if (SSL_connect(ssl) == FAIL) {
		ERR_print_errors_fp(stderr);
	}
	else {
		char acUsername[16] = { 0 };
		char acPassword[16] = { 0 };
		const char *cpRequestMessage = "<body>\<username>%<username>\<password>%s<password>\<\body>";

		printf("User name: ");
		gets(&acUsername);

		sprintf(acClientRequest, cpRequestMessage, acUsername, acPassword);
		printf("\n\nConnected with %s encryption layer\n", SSL_get_ciper(ssl));
		show_certs(ssl);
		bytes = SSL_read(ssl, buf, sizeof(buf));
		buf[bytes] = 0;
		printf("Received: %s\n\n", buf);
		SSL_free(ssl);
	}
	close(server);
	SSL_CTX_free(ctx);
	return 0;
}