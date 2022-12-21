#include <stdio.h>
#include <stdlib.h>
#include <ev.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include <linux/ip.h>  /* for ipv4 header */
#include <linux/udp.h> /* for upd header */

#define BUFFER_SIZE 100000
#define HEADER_SIZE (sizeof(struct iphdr) + sizeof(struct udphdr))
int total_clients = 0;
int receivecount = 0;
long size = 0;
clock_t start = 0, end = 0;
time_t now = 0;

/* Read client message */
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	char buffer[BUFFER_SIZE], tarIPstr[20] = {0}, temp[20] = {0};
	struct sockaddr_in connectedfrom;
	socklen_t connectedfromlen;
	ssize_t read;
	if (EV_ERROR & revents)
	{
		perror("got invalid event");
		return;
	}

	// Receive message from client socket
	connectedfromlen = sizeof(struct sockaddr_in);
	read = recvfrom(watcher->fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&connectedfrom, &connectedfromlen);
	inet_ntop(AF_INET, &connectedfrom.sin_addr.s_addr, tarIPstr, 512);
	if (read < 0)
	{
		perror("read error");
		return;
	}

	if (read == 0)
	{
		// Stop and free watchet if client socket is closing
		ev_io_stop(loop, watcher);
		free(watcher);
		perror("peer might closing");
		total_clients--; // Decrement total_clients count
		printf("%d client(s) connected.\n", total_clients);
		return;
	}
	else
	{
		buffer[read - 1] = '\0'; /* we need a null character at the end*/

		strncpy(temp, buffer + HEADER_SIZE, 6);
		//printf("buffer = %s\n", buffer+HEADER_SIZE);
		if (strcmp(temp, "hihihi") == 0)
		{
			if (receivecount == 0)
			{
				printf("hi\n");
				start = clock();
				now = time(NULL);
				printf("Receiving datagram from %s\n", tarIPstr);
			}
			receivecount++;
			//printf("\rreceivecount = %d time = %lds", receivecount, time(NULL) - now);
			size = size + read;
		}
		else if (strcmp(temp, "finfin") == 0 && receivecount != 0)
		{
			end = clock();
			now = time(NULL) - now;
			size = size + read;
			printf("receivecount = %d time = %lds", receivecount, now);
			printf("\ntotal received: %f Mbytes\nTime to receive:%lds\nUDP speed : %f Mb/s\n", size / 1000000.0, now, (size / 1000000.0) / (float)now);
			size = 0;
			receivecount = 0;
		}
		//printf("received packet\n");
	}

	// Send message bach to the client
	//send(watcher->fd, buffer, read, 0);
	bzero(buffer, read);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{ // check input
		fprintf(stderr, "Usage: %s [Server port]\n", argv[0]);
		exit(2);
	}
	else if (atoi(argv[1]) < 1025 || atoi(argv[1]) > 65535)
	{
		printf("Server port error: port must in range of 1025 to 65535!!\n");
		fprintf(stderr, "Usage: %s [Server port]\n", argv[0]);
		exit(2);
	}

	int ownport = atoi(argv[1]);
	int sock = -1, rc = 0;
	struct sockaddr_in sin;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(ownport);
	sin.sin_addr.s_addr = INADDR_ANY;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sock < 0)
		perror("Create raw socket failed");
	else
	{
		rc = bind(sock, (struct sockaddr *)&sin, sizeof(sin));
		if (rc < 0)
		{
			perror("bind failed");
			exit(1);
		}
	}

	struct ev_loop *loop = ev_default_loop(0);	   // initial the ev loop
	struct ev_io w_accept;						   // declair watcher
	ev_io_init(&w_accept, read_cb, sock, EV_READ); // Initialize and start a watcher to accepts client requests
	ev_io_start(loop, &w_accept);				   // Start watcher

	printf("Server started.\n");
	printf("Server port: %s\n", argv[1]);
	// Start infinite loop
	while (1)
	{
		ev_loop(loop, 0); // Run loop
	}

	return 0;
}
