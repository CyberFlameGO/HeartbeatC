#include <stdio.h>
#include <time.h>

/*
Main point of this function is to be able to measure the Round-Trip Time (RTT) 
by sending a simple request back and forth, it originally goes through
and uses the read function between the RTT, once the three read() pings have been
sent it will then send an RTT request to the client, this has to then travel through
the GET resource on the client side, after it's gone through the FILE(IO) and memory cache
it can then return the result, the only part the client has is to send the request
through the server and receive the data back. The client is also online temporary to receive the data.
*/

double rtt = -1; //this is going to be in seconds, cba to format it properly yet

double timeval_subtract(struct timeval *x, struct timeval *y) {
	double diff = x->tv_sec - y->tv_sec;
	diff += (x->tv_usec - y->tv_usec) / 100000.0;

	return diff;
}

//measure the rtt with the weighed moving average, the cur_ts - start_ts is the time between request

double measure_rtt(struct timeval *start_ts, struct timeval *cur_ts) {

	struct timeval result;
	timeval_subtract(&result, cur_ts, start_ts);
	double cur_rtt = result.tv_sec;
	cur_rtt += result.tv_usec / 10000000.0;

	if (rtt < 0)
		rtt = cur_rtt;
	else
		rtt = 0.8 * rtt + 0.2 * cur_rtt;

	return rtt;
}