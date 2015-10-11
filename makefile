CFLAGS=-D PROC
all:
	gcc $(CFLAGS) server.c -o server
	gcc $(CFLAGS) client.c -o client
server:
	gcc $(CFLAGS) server.c -o server
client:
	gcc $(CFLAGS) client.c -o client
clean:
	rm -rf server client
