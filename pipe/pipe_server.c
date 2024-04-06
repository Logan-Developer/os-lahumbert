#include <stdio.h> // Common for standard input/output

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#define PIPE1 _T("\\\\.\\pipe\\client_to_server.pipe")
#define PIPE2 _T("\\\\.\\pipe\\server_to_client.pipe")
#else
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FIFO1 "client_to_server.pipe"
#define FIFO2 "server_to_client.pipe"
#endif

int main() {
    char buffer[1024];

    #ifdef _WIN32
        DWORD bytesRead, bytesWritten;
        HANDLE hpipe = INVALID_HANDLE_VALUE;

        // Create the named pipes
        hpipe = CreateNamedPipe(
            TEXT("\\\\.\\pipe\\pipe1"), // Pipe name
            PIPE_ACCESS_DUPLEX,     // Read/write access
            PIPE_TYPE_MESSAGE |     // Message type pipe
            PIPE_READMODE_MESSAGE | // Message-read mode
            PIPE_WAIT,              // Blocking mode
            PIPE_UNLIMITED_INSTANCES, // Unlimited instances
            1024,                  // Outbound buffer size
            1024,                  // Inbound buffer size 
            0,                     // Default timeout
            NULL                   // Default security attributes
        );
        if (hpipe == INVALID_HANDLE_VALUE) {
            printf("Error creating pipe: %d\n", GetLastError());
            return 1;
        }

        // Connect to the pipe
        BOOL connected = ConnectNamedPipe(hpipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (!connected) {
            printf("Error connecting to pipe: %d\n", GetLastError());
            CloseHandle(hpipe);
            return 1;
        }

    #else // Linux/UNIX
        int read_fd, write_fd;

        // Create the named pipes (if they don't exist)
        mkfifo(FIFO1, 0666);
        mkfifo(FIFO2, 0666);

        // Open the pipes
        read_fd = open(FIFO1, O_RDONLY);  // Open client-to-server pipe for reading
        write_fd = open(FIFO2, O_WRONLY); // Open server-to-client pipe for writing
    #endif

    // Communication Logic (Keep this mostly platform-independent)
    char *helloMsg = "Hello, I am the server!, who are you?";
    #ifdef _WIN32
        WriteFile(hpipe, helloMsg, strlen(helloMsg) + 1, &bytesWritten, NULL);
    #else
        write(write_fd, helloMsg, strlen(helloMsg) + 1);
    #endif
    printf("Server sent: %s\n", helloMsg);

    #ifdef _WIN32
        ReadFile(hpipe, buffer, sizeof(buffer), &bytesRead, NULL);
    #else
        read(read_fd, buffer, sizeof(buffer)); 
    #endif
    printf("Client sent: %s\n", buffer);

    char *response = "Hello, I am the server, nice to meet you!";
    #ifdef _WIN32
        WriteFile(hpipe, response, strlen(response) + 1, &bytesWritten, NULL);
    #else
        write(write_fd, response, strlen(response) + 1);
    #endif
    printf("Server sent: %s\n", response);

   // Closing Pipes/Handles
   #ifdef _WIN32
        CloseHandle(hpipe);
   #else
        close(read_fd);
        close(write_fd);
   #endif

    return 0;
}
