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
    HANDLE read_handle, write_handle;
#else
    int read_fd, write_fd;
#endif

// Pipe Creation (Conditional)
#ifdef _WIN32
    // Create the named pipes
    write_handle = CreateNamedPipe(
        PIPE1,                // Pipe name
        PIPE_ACCESS_OUTBOUND, // Write access
        PIPE_TYPE_MESSAGE,    // Message-type pipe
        1,                    // Only one instance
        1024,                 // Outbound buffer size
        1024,                 // Inbound buffer size
        0,                    // Default timeout
        NULL                  // Default security attributes
    );
    if (write_handle == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("Error creating outbound pipe: %ld\n"), GetLastError());
        CloseHandle(write_handle);
        return 1;
    }

    read_handle = CreateNamedPipe(
        PIPE2,
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_MESSAGE,
        1,
        1024,
        1024,
        0,
        NULL);
    if (read_handle == INVALID_HANDLE_VALUE)
    {
        _tprintf(_T("Error creating inbound pipe: %ld\n"), GetLastError());
        CloseHandle(read_handle);
        return 1;
    }
#else
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);
#endif

// Pipe Opening/Connecting
#ifdef _WIN32
    // Connect to the outbound pipe (server-to-client)
    BOOL connected = ConnectNamedPipe(write_handle, NULL);
    if (!connected)
    {
        _tprintf(_T("Error connecting to outbound pipe: %ld\n"), GetLastError());
        CloseHandle(read_handle);
        CloseHandle(write_handle);
        return 1;
    }
    // Connect to the inbound pipe (client-to-server)
    connected = ConnectNamedPipe(read_handle, NULL);
    if (!connected)
    {
        _tprintf(_T("Error connecting to inbound pipe: %ld\n"), GetLastError());
        CloseHandle(read_handle);
        CloseHandle(write_handle);
        return 1;
    }
#else
    write_fd = open(FIFO1, O_WRONLY); // Open client-to-server pipe for writing
    read_fd = open(FIFO2, O_RDONLY);  // Open server-to-client pipe for reading
#endif

//  Communication logic (Keep this mostly platform-independent)
#ifdef _WIN32
    ReadFile(read_handle, buffer, sizeof(buffer), &bytesRead, NULL);
#else
    read(read_fd, buffer, sizeof(buffer));
#endif
    printf("Server sent: %s\n", buffer);

    char *msg = "Hello, I am the client, written by Logan";

#ifdef _WIN32
    WriteFile(write_handle, msg, strlen(msg) + 1, &bytesWritten, NULL);
#else
    write(write_fd, msg, strlen(msg) + 1);
#endif
    printf("Client sent: %s\n", msg);

#ifdef _WIN32
    ReadFile(read_handle, buffer, sizeof(buffer), &bytesRead, NULL);
#else
    read(read_fd, buffer, sizeof(buffer));
#endif
    printf("Server sent: %s\n", buffer);

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
