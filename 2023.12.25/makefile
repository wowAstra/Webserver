CC = g++
CFLAG = -Wall -c -pthread
VPATH += ../mytinymuduo
INCLUDES = -I ../mytinymuduo -I ./
OBJ_DIR := ./obj/
OBJS = $(OBJ_DIR)main.o $(OBJ_DIR)httpserver.o $(OBJ_DIR)httpresponse.o $(OBJ_DIR)httpcontent.o \
	   $(OBJ_DIR)httprequest.o $(OBJ_DIR)tcpserver.o $(OBJ_DIR)acceptor.o \
	   $(OBJ_DIR)channel.o $(OBJ_DIR)eventloop.o $(OBJ_DIR)epoller.o $(OBJ_DIR)tcpconnection.o \
	   $(OBJ_DIR)eventloopthreadpool.o $(OBJ_DIR)eventloopthread.o $(OBJ_DIR)thread.o $(OBJ_DIR)buffer.o
BUILD = -O3 -DNDEBUG

web_server : $(OBJS)
	$(CC) -pthread -I ./obj $^ -o $@ $(BUILD)

$(OBJ_DIR)main.o : main.cpp eventloop.h address.h httpserver.h httprequest.h httpresponse.h httpresponsefile.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)httpserver.o : httpserver.cpp httpserver.h buffer.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)httpresponse.o : httpresponse.cpp httpresponse.h buffer.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)httpcontent.o : httpcontent.cpp httpcontent.h httprequest.h httpparsestate.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)httprequest.o : httprequest.cpp httprequest.h httpparsestate.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)tcpserver.o : tcpserver.cpp tcpserver.h acceptor.h tcpconnection.h eventloopthreadpool.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)acceptor.o : acceptor.cpp acceptor.h address.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)channel.o : channel.cpp channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)eventloop.o : eventloop.cpp eventloop.h channel.h mutex.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)eventloopthreadpool.o : eventloopthreadpool.cpp eventloopthreadpool.h eventloopthread.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)eventloopthread.o : eventloopthread.cpp eventloopthread.h mutex.h condition.h eventloop.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)thread.o : thread.cpp thread.h latch.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)epoller.o : epoller.cpp epoller.h channel.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)tcpconnection.o : tcpconnection.cpp tcpconnection.h channel.h buffer.h httpcontent.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

$(OBJ_DIR)buffer.o : buffer.cpp buffer.h
	$(CC) $(INCLUDES) $(CFLAG) $< -o $@ $(BUILD)

.PHONY : clean
clean:
	rm web_server ./obj/*.o