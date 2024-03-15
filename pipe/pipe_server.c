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
        int bytesRead, bytesWritten;
        HANDLE read_handle, write_handle;

        // Create the named pipes
        read_handle = CreateNamedPipe(
            PIPE1,                 // Pipe name
            PIPE_ACCESS_INBOUND,   // Read access
            PIPE_TYPE_MESSAGE,     // Message-type pipe
            1,                     // Only one instance
            1024,                  // Outbound buffer size
            1024,                  // Inbound buffer size 
            0,                     // Default timeout
            NULL                   // Default security attributes
        );
        if (read_handle == INVALID_HANDLE_VALUE) {
            _tprintf(_T("Error creating inbound pipe: %d\n"), GetLastError());
            return 1;
        }

        write_handle = CreateNamedPipe(
            PIPE2,
            PIPE_ACCESS_OUTBOUND, 
            PIPE_TYPE_MESSAGE,
            1, 
            1024, 
            1024, 
            0,
            NULL 
        );
        if (write_handle == INVALID_HANDLE_VALUE) {
            _tprintf(_T("Error creating outbound pipe: %d\n"), GetLastError());
            CloseHandle(read_handle);
            return 1;
        }

        // Connect to the inbound pipe (client-to-server)
        BOOL connected = ConnectNamedPipe(read_handle, NULL);
        if (!connected) {
            _tprintf(_T("Error connecting to inbound pipe: %d\n"), GetLastError());
            CloseHandle(read_handle);
            CloseHandle(write_handle);
            return 1;
        }

        // Connect to the outbound pipe (server-to-client)
        connected = ConnectNamedPipe(write_handle, NULL);
        if (!connected) {
            _tprintf(_T("Error connecting to outbound pipe: %d\n"), GetLastError());
            CloseHandle(read_handle);
            CloseHandle(write_handle);
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
        WriteFile(write_handle, helloMsg, strlen(helloMsg) + 1, &bytesWritten, NULL);
    #else
        write(write_fd, helloMsg, strlen(helloMsg) + 1);
    #endif
    printf("Server sent: %s\n", helloMsg);

    #ifdef _WIN32
        ReadFile(read_handle, buffer, sizeof(buffer), &bytesRead, NULL);
    #else
        read(read_fd, buffer, sizeof(buffer)); 
    #endif
    printf("Client sent: %s\n", buffer);

    char *response = "Hello, I am the server, nice to meet you!";
    #ifdef _WIN32
        WriteFile(write_handle, response, strlen(response) + 1, &bytesWritten, NULL);
    #else
        write(write_fd, response, strlen(response) + 1);
    #endif
    printf("Server sent: %s\n", response);

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
