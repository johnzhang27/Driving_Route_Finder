CC = g++
OBJS = dijkstra.o digraph.o server.o serialport.o
CFLAGS=-c -Wall -O2
PROGRAM = server

ALL: server

server: server.o digraph.o dijkstra.o serialport.o
	$(CC) server.o digraph.o dijkstra.o serialport.o -o server

server.o: server.cpp
	$(CC) server.cpp -o server.o $(CFLAGS)

digraph.o: digraph.cpp
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

dijkstra.o: dijkstra.cpp
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

serialport.o: serialport.cpp
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)
	
clean:
	@rm -f $(OBJS)
	@rm -f $(PROGRAM)