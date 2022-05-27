#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 1024

void write_file(int sockfd, struct sockaddr_in addr)
{
    char* filename = "serverFTPlmao.txt";
    int rec;
    char buffer[SIZE];
    socklen_t addressSize;

    // TO create a file
    FILE* outfile = fopen(filename, "w");

    //Recieve the data and also write the data into the file
    while (1)
    {
        addressSize = sizeof(addr);
        rec = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, &addressSize);

        if (strcmp(buffer, "END") == 0)
        {
            break;
        }
        
        printf("(RECEIVING) The Data received from client: %s", buffer);
        fprintf(outfile, "%s", buffer);
        bzero(buffer, SIZE);
    }
    fclose(outfile);
}

int main()
{
    // To define the ip and the port
    char* ip = "192.168.50.164";
    const int port = 20;

    //define the variables
    int serverSock;
    struct sockaddr_in serverAddress, client_addr;
    char buffer[SIZE];
    int binder;
    
    // UDP Socket created
    serverSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverSock < 0)
    {
        perror("(ERROR!) socket has error");
        exit(1);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = port;
    serverAddress.sin_addr.s_addr = inet_addr(ip);

    binder = bind(serverSock, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (binder < 0)
    {
      perror("(ERROR!) bind error");
      exit(1);
    }

    printf("(STARTING) The UDP File server has started. \n");
    write_file(serverSock, client_addr);

    printf("\n(SUCCESS) Data transfer has been completed. \n");
    printf("(Closing) Closing by shutting down the server. \n");
    close(serverSock);

    return 0;
}