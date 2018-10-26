#include "../packages/pcap/pcap/pcap.h";
#include <stdio.h>

#define PCAP_ERRBUFF_SIZE 1
#define handle pcap_t
#define EN10MB 10
#define ETHER_ADDR_LEN 6
#define SIZE_ETHERNET 14
/*
This script is used to sniff and listen for network packets based on 
specified data (packet length, signed data etc) and filter it back into
a system to be reviewed. In order for it to work the script must be on
the server and not attempt to connect externally. 

>>> dependancies
>>> pcap
>>> tcplib [included in pcap.h]
*/

struct sniff_ethernet {
	u_char ether_dhost[ETHER_ADDR_LEN]; //destination host address
	u_char ether_shost[ETHER_ADDR_LEN]; //source host address
	u_short ether_type; //can be assigned to IP, ARP, RARP etc
};

struct sniff_ip {
	u_char ip_vhl;
	u_char ip_tos;
	u_short ip_len;
	u_short ip_id;
	u_short ip_off; //fragment offset field
#define IP_RF 0x8000
#define IP_DF 0x4000
#define IP_MF 0x2000
#define IP_OFFMASK 0x1FFF
	u_char ip_ttl;
	u_char ip_p;
	u_short ip_sum;
	struct in_addr ip_src, ip_dst;
};

#define IP_HL(ip) (((ip)->ip_vhl) &0x0f)
#define IP_V(ip) (((ip)->ip_vhl) >> 4)

typedef unsigned int tcp_seq;

struct sniff_tcp {
	u_short th_sport;	/* source port */
	u_short th_dport;	/* destination port */
	tcp_seq th_seq;		/* sequence number */
	tcp_seq th_ack;		/* acknowledgement number */
	u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;		/* window */
	u_short th_sum;		/* checksum */
	u_short th_urp;		/* urgent pointer */
};

const struct sniff_ethernet *ethernet;
const struct sniff_ip *ip;
const struct sniff_tcp *tcp;
unsigned int size_ip;
unsigned int size_tcp;
const char *payload;


int main(int argc, char *argv[]) {

	//ethernet, ip and tcp packet handling/modifications

	


	//type casting for the shit above

	ethernet = (struct sniff_ethernet*)(&payload);
	ip = (struct sniff_ip*)(&payload + SIZE_ETHERNET);

	if (size_ip < 20) {
		fprintf(stderr, "Invalid IP header length: %u bytes\n", size_ip);
		return;
	}

	tcp = (struct sniff_tcp*)(payload + SIZE_ETHERNET + size_ip);
	size_tcp = TH_OFF(tcp) * 4;

	if (size_tcp < 20) {
		fprintf(stderr, "Invalid TCP header length: %u bytes\n", size_tcp);
		return;
	}
	payload = (u_char *)(payload + SIZE_ETHERNET + size_ip + size_tcp);

	char *dev = argv[1];
	char errbuf[PCAP_ERRBUFF_SIZE];

	printf("Device: %s\n", dev);

	dev = pcap_lookupdev(errbuf);

	if (dev == NULL) {
		fprintf(stderr, "Couldn't find the default device: %s\n", errbuf);
		return 2;
	}

	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return 2;
	}

	if (pcap_datalink(handle) != EN10MB) {
		fprintf(stderr, "Device %s doesn't have ethernet headers available\n", dev);
	}

	int pcap_compile(pcap_addr_t *p, struct bpf_program *fp, char *str, int optimize, bpf_int32 netmask);
	int pcap_set_filter(pcap_addr_t *p, struct bpf_program *fp);

	//actual data we need to be able to sniff the network
	char dev[] = "r10";
	struct bpf_program *fp;
	char filter_exp[] = "port 23";
	bpf_u_int32 mask;
	bpf_u_int32 net;
	struct pcap_pkthdr  header; //header that pcap provides us
	const u_char *packet; //packet to sniff

	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't find mask for device: %s\n", dev);
		net = 0;
		mask = 0;
	}
	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		return 2;
	}

	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return 2;
	}

	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		return 2;
	}

	dev = pcap_lookupdev(errbuf);

	packet = pcap_next(handle, &header);
	printf("Found a packet with the length [%d]\n", header.len);
	pcap_close(handle);

	return 0;
}