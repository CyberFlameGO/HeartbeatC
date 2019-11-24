#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdbool.h>
#include<stdlib.h>
#include<inttypes.h>
#include<assert.h>

#ifdef linux 
#include<unistd.h>
#include<regex.h>
#include<dirent.h>
#include<linux/limits.h>
#endif

#define CPU_DATA_DIR "/proc/stat"
#define BATTERY_DATA_DIR "sys/class/power_supply"

//this shouldn't be needed for servers but if it's being ran on
//a laptop then it could be somewhat useful
void display_battery_life();
bool is_using_battery(); //used to check if the machine has a battery for the above function
void display_cpu_load();
void display_memory_usage();