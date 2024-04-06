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

int main()
{
    char buffer[1024];

#ifdef _WIN32
    DWORD bytesRead, bytesWritten;
    HANDLE hpipe;
#else
    int read_fd, write_fd;
#endif

// Pipe Creation (Conditional)
#ifdef _WIN32
    // Create the named pipe
    hPipe = CreateFile( 
         TEXT("\\\\.\\pipe\\pipe1"), // Pipe name
         GENERIC_READ |  // read and write access 
         GENERIC_WRITE, 
         0,              // no sharing 
         NULL,           // default security attributes
         OPEN_EXISTING,  // opens existing pipe 
         0,              // default attributes 
         NULL);          // no template file 

    if (hpipe == INVALID_HANDLE_VALUE)
    {
        printf("Error creating pipe: %ld\n", GetLastError());
        CloseHandle(hpipe);
        return 1;
    }
#else
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);
#endif

// Pipe Opening/Connecting
#ifdef _WIN32
    // no need to connect to the pipe, as it is already created
#else
    write_fd = open(FIFO1, O_WRONLY); // Open client-to-server pipe for writing
    read_fd = open(FIFO2, O_RDONLY);  // Open server-to-client pipe for reading
#endif

//  Communication logic (Keep this mostly platform-independent)
#ifdef _WIN32
    ReadFile(hpipe, buffer, sizeof(buffer), &bytesRead, NULL);
#else
    read(read_fd, buffer, sizeof(buffer));
#endif
    printf("Server sent: %s\n", buffer);

    char *msg = "Hello, I am the client, written by Logan";

#ifdef _WIN32
    WriteFile(hpipe, msg, strlen(msg) + 1, &bytesWritten, NULL);
#else
    write(write_fd, msg, strlen(msg) + 1);
#endif
    printf("Client sent: %s\n", msg);

#ifdef _WIN32
    ReadFile(hpipe, buffer, sizeof(buffer), &bytesRead, NULL);
#else
    read(read_fd, buffer, sizeof(buffer));
#endif
    printf("Server sent: %s\n", buffer);

// Closing Pipes/Handles
#ifdef _WIN32
    CloseHandle(hpipe);
#else
    close(read_fd);
    close(write_fd);
#endif

    return 0;
}
