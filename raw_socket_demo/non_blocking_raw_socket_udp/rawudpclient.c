/*
    Raw UDP sockets
*/
#include <arpa/inet.h>
#include <errno.h> //For errno - the error number
#include <netinet/in.h>
#include <netinet/ip.h>  //Provides declarations for ip header
#include <netinet/udp.h> //Provides declarations for udp header
#include <stdio.h>       //for printf
#include <stdlib.h>      //for exit(0);
#include <string.h>      //memset
#include <sys/socket.h>  //for socket ofcourse
#include <time.h>

/*
    96 bit (12 bytes) pseudo header needed for udp header checksum calculation
*/
struct pseudo_header {
  u_int32_t source_address;
  u_int32_t dest_address;
  u_int8_t placeholder;
  u_int8_t protocol;
  u_int16_t udp_length;
};

/*
    Generic checksum calculation function
*/
unsigned short
csum(unsigned short* ptr, int nbytes) {
  register long sum;
  unsigned short oddbyte;
  register short answer;

  sum = 0;
  while (nbytes > 1) {
    sum += *ptr++;
    nbytes -= 2;
  }
  if (nbytes == 1) {
    oddbyte = 0;
    *((u_char*)&oddbyte) = *(u_char*)ptr;
    sum += oddbyte;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum = sum + (sum >> 16);
  answer = (short)~sum;

  return (answer);
}

int
main(int argc, char** argv) {
  if (argc < 6) {
    printf(
      "Usage: sudo %s [Server IP] [Server Port] [Bytes of datagram] [Time to test] [spoofed IP]\n",
      argv[0]);
    exit(0);
  }
  // Create a raw socket of type IPPROTO
  int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

  if (s == -1) {
    // socket creation failed, may be because of non-root privileges
    perror("Failed to create raw socket");
    exit(1);
  }

  // Datagram to represent the packet
  char datagram[5000000], source_ip[32], *data, *pseudogram;

  // zero out the packet buffer
  memset(datagram, 0, 5000000);

  // IP header
  struct iphdr* iph = (struct iphdr*)datagram;

  // UDP header
  struct udphdr* udph = (struct udphdr*)(datagram + sizeof(struct ip));

  struct sockaddr_in sin;
  struct pseudo_header psh;

  // Data part
  data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
  strcpy(data, "hihihi");
  memset(data + 6, 'x', atoi(argv[3]) - 34);

  // some address resolution
  strcpy(source_ip, argv[5]);

  sin.sin_family = AF_INET;
  sin.sin_port = htons(atoi(argv[2]));
  sin.sin_addr.s_addr = inet_addr(argv[1]);

  // Fill in the IP Header
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 0;
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
  iph->id = 1; // Id of this packet
  iph->frag_off = 0;
  iph->ttl = 255;
  iph->protocol = IPPROTO_UDP;
  iph->check = 0;                    // Set to 0 before calculating checksum
  iph->saddr = inet_addr(source_ip); // Spoof the source ip address
  iph->daddr = sin.sin_addr.s_addr;

  // UDP header
  udph->source = htons(2222);
  udph->dest = htons(atoi(argv[2]));
  udph->len = htons(8 + strlen(data)); // tcp header size
  udph->check = 0;                     // leave checksum 0 now, filled later by pseudo header

  // Now the UDP checksum using the pseudo header
  psh.source_address = inet_addr(source_ip);
  psh.dest_address = sin.sin_addr.s_addr;
  psh.placeholder = 0;
  psh.protocol = IPPROTO_UDP;
  psh.udp_length = htons(sizeof(struct udphdr) + strlen(data));

  int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
  pseudogram = malloc(psize);

  memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
  memcpy(pseudogram + sizeof(struct pseudo_header), udph, sizeof(struct udphdr) + strlen(data));

  // Ip checksum
  iph->check = csum((unsigned short*)datagram, iph->tot_len);
  udph->check = csum((unsigned short*)pseudogram, psize);

  // loop if you want to flood :)
  int start = time(NULL);
  int diff = 0;
  int count = 0;

  printf("Target server: %s:%s\n", argv[1], argv[2]);
  printf("Bytes of each datagram: %d\n", iph->tot_len);
  printf("Time to send datagram: %s\n", argv[4]);
  printf("Sending datagram..\n");
  while (diff < atoi(argv[4])) {
    int now = time(NULL);
    diff = now - start;
    // Send the packet
    if (sendto(s, datagram, iph->tot_len, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
      perror("sendto failed");
      exit(0);
    }
    // Data send successfully
    else {
      count++;
      // printf ("Packet Send. Length : %d \n" , iph->tot_len);
    }
  }
  strcpy(data, "finfin");
  int counter = 0;
  while (counter < 1000000) {
    counter++;
    if (sendto(s, datagram, iph->tot_len, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
      perror("sendto failed");
    }
  }
  printf("sent %d packets\n", count);
  return 0;
}

// Complete
