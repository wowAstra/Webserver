.PHONY:all
all:server client

server: server.cpp chatroom.h processpool.h
	g++ server.cpp -o server
client: client.cpp
	g++ client.cpp -o client

.PHONY:clean
clean:
	rm -f server client