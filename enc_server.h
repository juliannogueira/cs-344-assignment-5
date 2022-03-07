#ifndef ENC_SERVER_H
#define ENC_SERVER_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

struct Server {
    int MAX_CONNECTIONS;
    int port;
    int is_running;
    int socket;
    struct sockaddr_in *address;
    socklen_t size;
};

struct Client {
    int BUFFER_SIZE;
    int socket;
    int recv_length;
    int send_length;
    int index;
    int message_length_int;
    char *buffer;
    char *message_length_str;
    struct sockaddr_in *address;
    socklen_t size;
};

void init_server(struct Server *server, int port);

void free_server(struct Server *server);

void init_client(struct Client *client);

void free_client(struct Client *client);

void init_address(struct sockaddr_in* address, int port);

void run_server(int port); 

void process_request(struct Client *client);

#endif