#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef linux 
#include <unistd.h>
#include <sys/resource.h>
#endif

void calculate_cpu_usage(char* argv[]);
void calculate_ram_usage();