.PHONY:all
all: server

server: server.cpp echo.h processpool.h
	g++ server.cpp -o server

.PHONY:clean
clean:
	rm -f server