#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef linux 
#include <unistd.h>
#endif

void calculate_cpu_usage(char* argv[]);