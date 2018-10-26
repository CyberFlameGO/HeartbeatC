#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024*1024*2 // 2MB
#define SKIPS 10 // initial measurement skips - skip the first slow-start values + initial burst
#define ROUNDS 2 // measurement rounds - number of file downloads
#define TIMEOUT 2 // socket timeout in s

// connection specifications
char *domain = ""; 
char *resource = ""; 
int port = 80;

double bandwidth[2] = { -1, -1 };
float total_bytes[2] = { 0, 0 };
int n[2] = { 0, 0 };
int skips = SKIPS;
double rtt = -1;
int rounds = ROUNDS;

double timeval_subtract(struct timeval *x, struct timeval *y)
{
	double diff = x->tv_sec - y->tv_sec;
	diff += (x->tv_usec - y->tv_usec) / 1000000.0;

	return diff;
}

double measure_bw(struct timeval *start, struct timeval *cur, float bytes, int option) {
	total_bytes[option] += bytes;
	double ts_diff = timeval_subtract(cur, start);
	double cur_bw = (total_bytes[option] / (1024 * 1024) / ts_diff);

	if (bandwidth[option] < 0) {
		bandwidth[option] = cur_bw;
	}
	else {
		bandwidth[option] = (n[option] + 1) / ((n[option] / bandwidth[option]) + (1 / cur_bw));
	}

	n[option]++;
	return bandwidth[option];
}

double measure_rtt(struct timeval *start_ts, struct timeval *cur_ts)
{
	double cur_rtt = timeval_subtract(cur_ts, start_ts);

	if (rtt < 0)
	{
		// first measurement
		rtt = cur_rtt;
	}
	else
	{
		// weighed moving average
		rtt = 0.8*rtt + 0.2*cur_rtt;
	}

	return rtt;
}

void make_request(int socket, char *buf, int size) {
	bzero(buf, size);
	sprintf(buf, "GET %s HTTP/1.1\\R\\Host: %s\r\n\n\n", resource, domain);

	if (send(socket, buf, strlen(buf), 0) < 0) {
		perror("Error while sending request");
		return;
	}
}

int create_tcp_connect() {
	int sock;
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error while creating socket");
		return -1;
	}
	struct hostent *server;
	server = gethostbyname(domain);
	if (server == NULL) {
		perror("Could not locate server\n");
		return -1;
	}

	struct sockaddr_in serveraddr;

	bzero((char *)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		perror("Could not create connection to server");
		return -1;
	}
	return sock;
}