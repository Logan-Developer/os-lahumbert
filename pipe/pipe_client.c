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
        HANDLE read_handle, write_handle;
    #else
        int read_fd, write_fd;
    #endif

    // Pipe Creation (Conditional)
    #ifdef _WIN32 
        // ... (Windows named pipe creation code)
    #else
        mkfifo(FIFO1, 0666);
        mkfifo(FIFO2, 0666);
    #endif 

    // Pipe Opening/Connecting
    #ifdef _WIN32
         // ... (Windows code for connecting to named pipes)
    #else
        read_fd = open(FIFO1, O_RDONLY);  // Open client-to-server pipe for reading
        write_fd = open(FIFO2, O_WRONLY); // Open server-to-client pipe for writing
    #endif

    //  Communication logic (Keep this mostly platform-independent) 
    #ifdef _WIN32
        ReadFile(read_handle, buffer, sizeof(buffer), &bytesRead, NULL);
    #else
        read(read_fd, buffer, sizeof(buffer)); 
    #endif
    printf("Server sent: %s\n", buffer); 

    char* msg = "Hello, I am the client, written by Logan";

    #ifdef _WIN32
        WriteFile(write_handle, msg, strlen(msg) + 1, &bytesWritten, NULL);
    #else
        write(write_fd, msg, strlen(msg) + 1);
    #endif
    printf("Client sent: %s\n", msg);

   // Closing Pipes/Handles
   #ifdef _WIN32
        CloseHandle(read_handle);
        CloseHandle(write_handle);
   #else
        close(read_fd);
        close(write_fd);
   #endif

    return 0;
}
