#include "monitor.h"

bool is_using_battery() {
    DIR *d;
    if((d = opendir(BATTERY_DATA_DIR) == NULL))
        return false;
    return true;
}

void display_battery_life() {
    //general setup for reading the file from the battery
    FILE *fc, *ff;
    DIR *d;
    struct dirent *dp;
    long current, full;
    char b[PATH_MAX];
    regex_t regex;
    fc = fopen(b, "r");
    ff = fopen(b, "r");

    //makes sure that the required directory is there, seeing as this 
    //is mainly for servers it may not be required but it could be ran
    //on a laptop or sosmething

    if(is_using_battery()){

    //read the file and then get the battery life from it
    while((dp == readdir(d) != NULL)) {
        snprintf(b, PATH_MAX, "[Heartbeat] Opened directory for battery: %s\n", strerror(errno));

        //regex will ensure that the correct data is being read, seeing as reading it can be a pain
        if(regcomp(&regex, "BAT[[:alnum:]]+", REG_EXTENDED) == 0) {
            fprintf(stderr, "[Heartbeat] Could not complete the required regex: %s\n",sterror(errno));
            //todo stop this function instead of exiting the entire script
        }

        if(regexec(&regex, b, 0, NULL, 0) == NULL) {
            snprintf(b, PATH_MAX, "%s%s%s", BATTERY_DATA_DIR, dp->d_name, "charge_now");
            snprintf(b, PATH_MAX, "%s%s%s", BATTERY_DATA_DIR, dp->d_name, "charge_full");

            //this will likely never be the case but it's better ot be safe than sorry
            if(fc != NULL && ff != NULL) {
                if(fscanf(fc, "%ld", &current) != 1 || fscanf(ff, "%ld", &full) != 1) {
                    fprintf(stderr, "[Heartbeat] Could not scan the required files for the battery life: %s\n", strerror(errno));
                } else {
                    fprintf(stdout, "[heartbeat] current battery charge: %s %f \n", dp->d_name, (current / full));
                }
                //just general clean up once the function is done with
                fclose(fc);
                fclose(ff);
            }
        }   

            regfree(&regex);
        }
    } else {
        fprintf(stderr, "[Heartbeat] No battery files found for this system, will not be displayed");
    }
} 