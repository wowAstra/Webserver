CC = g++
CFLAG = -Wall -c
VPATH = src:../mytinymuduo:./obj
INCLUDES = -I ../mytinymuduo
OBJ_DIR := ./obj/
OBJS = main.o echo.o tcpserver.o acceptor.o \
	   channel.o eventloop.o epoller.o tcpconnection.o

web_server : $(OBJS)
	$(CC) -I ./obj $^ -o $@

main.o : main.cpp eventloop.h address.h echo.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

echo.o : echo.cpp echo.h tcpserver.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

tcpserver.o : tcpserver.cpp tcpserver.h acceptor.h tcpconnection.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

acceptor.o : acceptor.cpp acceptor.h address.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

channel.o : channel.cpp channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

eventloop.o : eventloop.cpp eventloop.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

epoller.o : epoller.cpp epoller.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

tcpconnection.o : tcpconnection.cpp tcpconnection.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

.PHONY : clean
clean:
	rm ./obj/*.o web_server