#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<regex.h>
#include<dirent.h>
#include<linux/limits.h>


#define BATTERY_DATA_DIR "sys/class/power_supply"

//this shouldn't be needed for servers but if it's being ran on
//a laptop then it could be somewhat useful
void display_battery_life();