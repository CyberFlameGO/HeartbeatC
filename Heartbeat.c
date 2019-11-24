#include "monitor.h"

int main(void) {

    fprintf(stdout, "----- [HEARTBEAT] -----");

    while (true) {
        display_battery_life();
        display_cpu_load();
        display_memory_usage();
        signal(SIGALARM, main);
        alarm(30);
    }
    return 0;
}