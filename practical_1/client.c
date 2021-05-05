#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 3000

int main()
{
    int socket_fd, addr_len = 0;
    struct sockaddr_in address; // server address
    char buffer[1024] = {0};
    // create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        printf("socket creation fail.");
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_len = sizeof(address);
    // connect to server
    if (connect(socket_fd, (struct sockaddr *)&address, addr_len) < 0)
    {
        printf("connection to server fail.");
        return -1;
    }

    while (1)
    {
        char message[1024];
        char buffer[1024] = {0};
        printf("Enter message: ");
        gets(message);
        fflush(stdin);
        printf("sending message to server...\n", message);
        if (write(socket_fd, message, strlen(message)) < 0)
        {
            printf("message sending fail.");
            break;
        }
        if (strcmp(message, "exit") == 0)
            break;
        if (read(socket_fd, buffer, 1024) < 0)
        {
            printf("message receiving fail.");
            break;
        }
        printf("server: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0)
            break;
    }
    close(socket_fd);
    return 0;
}