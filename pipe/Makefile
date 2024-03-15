CFLAGS := -g -Wall  # Common compiler flags

ifeq ($(OS),Windows_NT)  # Check for Windows
    CFLAGS += -D_WIN32
endif

all: server.out client.out 

server.out: pipe_server.c
	gcc $(CFLAGS) -o server.out pipe_server.c $(LDLIBS)

client.out: pipe_client.c
	gcc $(CFLAGS) -o client.out pipe_client.c $(LDLIBS)

clean:
	rm -f server.out client.out client_to_server.pipe server_to_client.pipe
