CC = g++
CFLAG = -Wall -c
VPATH = src:../mytinymuduo:./obj
INCLUDES = -I ../mytinymuduo
OBJ_DIR := ./obj/
OBJS = main.o echo.o tcpserver.o acceptor.o \
	   channel.o eventloop.o epoller.o tcpconnectionptr.o

web_server : $(OBJS)
	$(CC) -I ./obj $^ -o $@

main.o : main.cpp eventloop.h address.h echo.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

echo.o : echo.cpp echo.h tcpserver.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

tcpserver.o : tcpserver.cpp tcpserver.h acceptor.h tcpconnectionptr.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

acceptor.o : acceptor.cpp acceptor.h address.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

channel.o : channel.cpp channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

eventloop.o : eventloop.cpp eventloop.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

epoller.o : epoller.cpp epoller.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

tcpconnectionptr.o : tcpconnectionptr.cpp tcpconnectionptr.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $(OBJ_DIR)$@

.PHONT : clean
clean:
	./target/web_server ./obj/*.o