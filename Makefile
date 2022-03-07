CC = gcc --std=gnu99 -pthread
CFLAGS = -g -Wall
ENC_SERVER = enc_server
ENC_CLIENT = enc_client
KEYGEN = keygen

all: enc_server enc_client keygen

enc_server: enc_server.o util.o
	$(CC) $(CFLAGS) enc_server.o util.o -o $(ENC_SERVER)

enc_server.o: enc_server.h enc_server.c
	$(CC) $(CFLAGS) -c enc_server.c

enc_client: enc_client.o util.o
	$(CC) $(CFLAGS) enc_client.o util.o -o $(ENC_CLIENT)

enc_client.o: enc_client.h enc_client.c
	$(CC) $(CFLAGS) -c enc_client.c

keygen: keygen.o util.o
	$(CC) $(CFLAGS) keygen.o util.o -o $(KEYGEN)

keygen.o: keygen.h keygen.c
	$(CC) $(CFLAGS) -c keygen.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f *.o $(ENC_SERVER) $(ENC_CLIENT) $(KEYGEN)
