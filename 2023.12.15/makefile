CC = g++
CFLAG = -Wall -c -pthread
VPATH += ../mytinymuduo
INCLUDES = -I ../mytinymuduo
OBJ_DIR := ./obj/
OBJS = $(OBJ_DIR)main.o $(OBJ_DIR)echo.o $(OBJ_DIR)tcpserver.o $(OBJ_DIR)acceptor.o \
	   $(OBJ_DIR)channel.o $(OBJ_DIR)eventloop.o $(OBJ_DIR)epoller.o $(OBJ_DIR)tcpconnection.o\
	   $(OBJ_DIR)eventloopthreadpool.o $(OBJ_DIR)eventloopthread.o $(OBJ_DIR)thread.o 

web_server : $(OBJS)
	$(CC) -pthread -I ./obj $^ -o $@

$(OBJ_DIR)main.o : main.cpp eventloop.h address.h echo.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)echo.o : echo.cpp echo.h tcpserver.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)tcpserver.o : tcpserver.cpp tcpserver.h acceptor.h tcpconnection.h eventloopthreadpool.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)acceptor.o : acceptor.cpp acceptor.h address.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)channel.o : channel.cpp channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)eventloop.o : eventloop.cpp eventloop.h channel.h mutex.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)eventloopthreadpool.o : eventloopthreadpool.cpp eventloopthreadpool.h eventloopthread.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)eventloopthread.o : eventloopthread.cpp eventloopthread.h mutex.h condition.h eventloop.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)thread.o : thread.cpp thread.h latch.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)epoller.o : epoller.cpp epoller.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

$(OBJ_DIR)tcpconnection.o : tcpconnection.cpp tcpconnection.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@

.PHONY : clean
clean:
	rm web_server ./obj/*.o