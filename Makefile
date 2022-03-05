CC = gcc --std=gnu99 -pthread
CFLAGS = -g -Wall
ENC_SERVER = enc_server
KEYGEN = keygen

all: enc_server keygen

enc_server: enc_server.o
	$(CC) $(CFLAGS) enc_server.o -o $(ENC_SERVER)

enc_server.o: enc_server.h enc_server.c
	$(CC) $(CFLAGS) -c enc_server.c

keygen: keygen.o util.o
	$(CC) $(CFLAGS) keygen.o util.o -o $(KEYGEN)

keygen.o: keygen.h keygen.c
	$(CC) $(CFLAGS) -c keygen.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f *.o $(KEYGEN) $(ENC_SERVER)
