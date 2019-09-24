CC = gcc
CFLAGS = -std=c99 -Wall
OBJECTS = UDPServer.o UDPClient.o UDP_Tx_Rx.o
EXECS = Server Client

server: Server

client: Client

Server : UDPServer.o UDP_Tx_Rx.o
	$(CC) $(CFLAGS) -o Server UDPServer.o UDP_Tx_Rx.o

Client : UDPClient.o UDP_Tx_Rx.o
	$(CC) $(CFLAGS) -o Client UDPClient.o UDP_Tx_Rx.o

clean:
	-rm $(OBJECTS) $(EXECS)
