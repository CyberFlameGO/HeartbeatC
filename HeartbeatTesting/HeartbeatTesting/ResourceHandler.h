#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef linux 
#include <unistd.h>
#endif

void calculate_cpu_usage(char* argv[]);