//
// Created by George Marr on 22/10/2018.
//

#include "../packages/pcap/pcap/pcap.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv){
    char *device;
    char ip[13];
    char subnet_mask[13];
    bpf_u_int32 ip_raw; //as an integer
    bpf_u_int32 subnet_mask_raw; //subnet as an int
    int lookup_return_code;
    char error_buffer[PCAP_ERRBUF_SIZE];
    struct in_addr address;

    //device finding
    device = pcap_lookupdev(error_buffer);
    if(device == NULL){
        printf("%s\n", error_buffer);
        return 1;
    }

    //return with device info
    lookup_return_code = pcap_lookupnet(device, &ip_raw, &subnet_mask_raw, error_buffer);

    if(lookup_return_code == -1){
        printf("%s\n", error_buffer);
        return 1;
    }

    /*
     * When calling inet_ntao() more than once it will overwrite the buffer. If the
     * data is stored in a pointer to the string returned by inet_ntoa() it can then be called
     * again later for the subnet mask. The first pointer is related to the ip address
     * which will actually have the contents of the subnet mask. This is why a string
     * copy and grab of the contents is being used. The pointer that is returned by
     * inet_ntoa() is always the same.
     *
     * The inet_ntoa() function converts the internet host address in given network
     * byte order, to a string in IPv4 dotted decimal notation(s). The string is
     * returned in a statically allocated buffer which subsequent calls overwrite.
     */


    //retrieve the ip so its somewhat readable
    address.s_addr = ip_raw;

    strcpy(ip, inet_ntoa(address));

    if(ip == NULL) {
        perror("inet_ntoa");
        return 1;
    }

    //same with the subnet
    address.s_addr = subnet_mask_raw;

    strcpy(subnet_mask, inet_ntoa(address));

    if(subnet_mask == NULL){
        perror("inet_ntoa");
        return 1;
    }

    printf("Device(s) found: %s\n", device);
    printf("IP address: %s\n", ip);
    printf("Subnet mask: %s\n", subnet_mask);

    return 0;
}