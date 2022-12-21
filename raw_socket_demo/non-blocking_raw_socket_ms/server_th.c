// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
// thread
#include <pthread.h>

#include <fcntl.h>
#include <arpa/inet.h>

void *job(void *);
void *printSpeed(void *);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    pthread_t t_;
    pthread_create(&t_, NULL, printSpeed, NULL); // 建立子執行緒

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("10.33.7.40");
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_t t; // 宣告 pthread 變數

        pthread_create(&t, NULL, job, &new_socket); // 建立子執行緒
    }
    return 0;
}

void *job(void *client_socket)
{
    int valread;
#define BUFSIZE 1024
    char buffer[BUFSIZE] = {0};
    char *hello = "Hello from server";
    int new_socket = *(int *)client_socket;

    int msgsize = 0;
    size_t bytes_read;

    int n = 0;

    valread = read(new_socket, buffer, BUFSIZE);

    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    //printf("Hello message sent\n");
    //close(*client_socket);
    pthread_exit(NULL); // 離開子執行緒
}

void *printSpeed(void *data)
{
    const char *filename = "speed";
    int fd, readData;

    char buffer[1024] = {0};
    while (1)
    {
        fd = open(filename, O_RDONLY);

        if (fd < 0)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        read(fd, buffer, sizeof(buffer));
        readData = atoi(buffer);
        sleep(readData);
        printf("speed = %d\n", readData);
        close(fd);
    }
}