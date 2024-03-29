CC = gcc --std=gnu99 -pthread
CFLAGS = -g -Wall
KEYGEN = keygen
ENC_SERVER = enc_server
ENC_CLIENT = enc_client
DEC_SERVER = dec_server
DEC_CLIENT = dec_client

all: keygen enc_server enc_client dec_server dec_client

keygen: keygen.o util.o
	$(CC) $(CFLAGS) keygen.o util.o -o $(KEYGEN)

keygen.o: keygen.h keygen.c
	$(CC) $(CFLAGS) -c keygen.c

enc_server: enc_server.o util.o
	$(CC) $(CFLAGS) enc_server.o util.o -o $(ENC_SERVER)

enc_server.o: enc_server.h enc_server.c
	$(CC) $(CFLAGS) -c enc_server.c

enc_client: enc_client.o util.o
	$(CC) $(CFLAGS) enc_client.o util.o -o $(ENC_CLIENT)

enc_client.o: enc_client.h enc_client.c
	$(CC) $(CFLAGS) -c enc_client.c

dec_server: dec_server.o util.o
	$(CC) $(CFLAGS) dec_server.o util.o -o $(DEC_SERVER)

dec_server.o: dec_server.h dec_server.c
	$(CC) $(CFLAGS) -c dec_server.c

dec_client: dec_client.o util.o
	$(CC) $(CFLAGS) dec_client.o util.o -o $(DEC_CLIENT)

dec_client.o: dec_client.h dec_client.c
	$(CC) $(CFLAGS) -c dec_client.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f *.o $(ENC_SERVER) $(ENC_CLIENT) $(DEC_SERVER) $(DEC_CLIENT) $(KEYGEN)
