#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file_data(FILE* infile, int sock, struct sockaddr_in address)
{
    int sender;
    char buffer[SIZE];

    // this is to send the data to the server
    while (fgets(buffer, SIZE, infile) != NULL)
    {
        printf("[SENDING DATA] %s\n", buffer);

        sender = sendto(sock, buffer, SIZE, 0, (struct sockaddr*)&address, sizeof(address));
        if (sender == -1)
        {
            perror("[ERROR!] Failed on send data to the server.");
            exit(1);
        }
        bzero(buffer, SIZE);
    }
    // terminating the session
    strcpy(buffer, "END");
    sendto(sock, buffer, SIZE, 0, (struct sockaddr*)&address, sizeof(address));

    fclose(infile);
}

int main(void)
{
    // The ip and port
    char *ip = "192.168.50.164";
    const int port = 20;

    int sock;
    struct sockaddr_in address;
    char *filename = "clientFTPlmao.txt";
    FILE *infile = fopen(filename, "r");

    //creating the UDP 
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("(ERROR!) Error on finding the socket");
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_port = port;
    address.sin_addr.s_addr = inet_addr(ip);

    //reading the file
    if (infile == NULL)
    {
        perror("(ERROR!) Failed reading the file");
        exit(1); 
    }

    // the data is sent to the server
    send_file_data(infile, sock, address);

    printf("(SUCCESS) Completed the data transfer. \n");
    printf("(CLOSING) The data from the server will be disconnected. \n");

    close(sock);

    return 0;
}