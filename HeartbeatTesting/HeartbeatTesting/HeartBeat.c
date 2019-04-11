#include "ResourceHandler.h"

#define NUM_SEC 3

int main(int argc, char* argv[]) {
	printf("---------- HEARTBEAT ----------");
	calculate_cpu_usage(argv);
	calculate_ram_usage();
	printf("---------- HEARTBEAT ----------");
}