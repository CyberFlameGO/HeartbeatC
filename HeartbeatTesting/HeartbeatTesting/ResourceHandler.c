#include "ResourceHandler.h"

void calculate_cpu_usage(char *argv[]) {
	char str[100];
	const char d[2] = " ";
	char* token;
	int i = 0, times, lag;
	long int sum = 0, idle, lastSum = 0, lastIdle = 0;
	long double idleFraction;

	times = atoi(argv[1]);
	lag = atoi(argv[2]);

	//open up the file that contains the system information
	while (times > 0) {
		FILE* fp = fopen("/proc/stat", "r");

		i = 0;
		//read the required information
		fgets(str, 100, fp);
		fclose(fp);
		//tok the str to the d to create the new token
		token = strtok(str, d);

		//if the token is not null then we can continue on our way
		while (token != NULL) {
			token = strtok(NULL, d);

			if (token != NULL)
				sum += atoi(token);
			if (i == 3)
				idle = atoi(token);
			i++;
		}
	}
	//some shit maths to determine what we're the outcome will be
	printf("\n Idle for %1f %% of the time.", (1.0 - (idle - lastIdle) * 1.0 / (sum - lastSum)) * 100);

	lastIdle = idle;
	lastSum = sum;
	times--;
	Sleep(lag);
}