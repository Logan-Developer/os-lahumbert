#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO1 "client_to_server.pipe"
#define FIFO2 "server_to_client.pipe"

int main() {
    char buffer[1024];
    int read_fd, write_fd;

    // Open the named pipes
    write_fd = open(FIFO1, O_WRONLY); // Open client-to-server pipe for writing
    read_fd = open(FIFO2, O_RDONLY); // Open server-to-client pipe for reading

    // Get message from the server
    read(read_fd, buffer, sizeof(buffer));
    printf("Server sent: %s\n", buffer);

    // Send a message to the server
    char* msg = "Hello, I am the client, writen by Logan";
    write(write_fd, msg, strlen(msg) + 1);
    printf("Client sent: %s\n", msg);

    // Read the response from the server
    read(read_fd, buffer, sizeof(buffer));
    printf("Server sent: %s\n", buffer);

    // Close the pipes
    close(read_fd);
    close(write_fd);
    return 0;
}
