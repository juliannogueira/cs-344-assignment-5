#ifndef ENC_CLIENT_H
#define ENC_CLIENT_H

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

struct Client {
    int port;
    int socket;
    int data_length;
    int key_length;
    int index;
    char *token;
    char *buffer;
    char *data_length_str;
    struct sockaddr_in *server_address;
    socklen_t server_size;
};

void init_address(struct sockaddr_in* address, int port, char* hostname);

void init_client(struct Client *client, int port); 

void free_client(struct Client *client);

void run_client(char *plaintext, char *key, int port);

void read_data(struct Client *client, char *filename);

void read_key(struct Client *client, char *filename);

#endif