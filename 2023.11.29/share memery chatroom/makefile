.PHONY:all
all:server client

server: server.cpp
	g++ server.cpp -o server -lrt
client: client.cpp
	g++ client.cpp -o client

.PHONY:clean
clean:
	rm -f server client