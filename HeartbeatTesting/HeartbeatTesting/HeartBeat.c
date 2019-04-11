#include "ResourceHandler.h"

#define NUM_SEC 60 //run every 60 seconds

int main(int argc, char* argv[]) {

	//clock for everything
	time_t lasttime, thistime;
	int i;

	lasttime = time(NULL);

	for (i = 0; i < 5; i++) {
		while (1) {
			thistime = time(NULL);
			if (thistime - lasttime >= NUM_SEC)
				break;
			if (thistime - lasttime >= 2)
				sleep(thistime - lasttime - 1);

		}

		//all of the code written for it goes here
		printf("---------- HEARTBEAT ----------");
		calculate_cpu_usage(argv);
		calculate_ram_usage();
		printf("---------- HEARTBEAT ----------");
		lasttime += NUM_SEC;
	}

}