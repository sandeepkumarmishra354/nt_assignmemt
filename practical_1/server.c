#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 3000

int main()
{
    int server_fd, new_client, addr_len = 0;
    struct sockaddr_in address, client_addr;
    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        printf("socket creation fail.");
        return -1;
    }
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    addr_len = sizeof(address);
    // bind this socket to appropriate port and address
    if (bind(server_fd, (struct sockaddr *)&address, addr_len) < 0)
    {
        printf("socket binding fail.");
        return -1;
    }
    // listen for incomming connections
    if (listen(server_fd, 10) < 0)
    {
        printf("socket listen fail.");
        close(server_fd);
        return -1;
    }

    while (1)
    {
        // accept new connection
        int cli_addr_len = sizeof(client_addr);
        printf("waiting for new connection...\n");
        new_client = accept(server_fd, (struct sockaddr *)&client_addr, &cli_addr_len);
        if (new_client < 0)
        {
            printf("incomming connection not accpeted.");
            break;
        }
        printf("client connected...\n");
        while (1)
        {
            printf("waiting for message...\n");
            char buffer[1024] = {0};
            char message[1024];
            int read_byte_count = read(new_client, buffer, 1024);
            if (read_byte_count == -1)
            {
                printf("data byte reading fail.");
                close(new_client);
                break;
            }
            printf("client: %s\n", buffer);
            if (strcmp(buffer, "exit") == 0)
                break;
            printf("Enter message: ");
            gets(message);
            fflush(stdin);
            printf("sending message to client...\n", message);
            write(new_client, message, 1024);
            if (strcmp(message, "exit") == 0)
                break;
        }
    }
    close(server_fd);
    return 0;
}