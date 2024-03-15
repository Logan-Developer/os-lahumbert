all: server.out client.out

server.out: pipe_server.c
	gcc -o server.out pipe_server.c

client.oout: pipe_client.c
	gcc -o client.out pipe_client.c

clean:
	rm -f server.out client.out client_to_server.pipe server_to_client.pipe