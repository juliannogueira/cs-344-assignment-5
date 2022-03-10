#include "enc_server.h"

/*
 * Run the server at the specified port.
 */
int main(int argc, char **argv){
    if (argc < 2) {
        error("Error: expected one argument, port\n", 2);
    } else {
        run_server(string_to_integer(*(argv + 1)));
    } 

    return 0;
}

/*
 * Initialize the server struct.
 */
void init_server(struct Server *server, int port) {
    server->MAX_CONNECTIONS = 5;
    server->port = port;
    server->is_running = 1;
    server->socket = -1;
    server->address = malloc(sizeof(struct sockaddr_in));
    server->size = sizeof(*(server->address));
}

/*
 * Free dynamically allocated memory in the server struct, then free the server
 * struct, itself.
 */
void free_server(struct Server *server) {
    free(server->address);
    free(server);
}

/*
 * Initialize the client struct.
 */
void init_client(struct Client *client) {
    client->socket = -1;
    client->recv_length = 0;
    client->send_length = 0;
    client->data_length = 0;
    client->index = 0;
    client->token = malloc(sizeof(char));
    client->buffer = malloc(sizeof(char) * 256);
    client->data_length_str = malloc(sizeof(char) * 16);
    client->address = malloc(sizeof(struct sockaddr_in));
    client->size = sizeof(*(client->address));
    memset(client->buffer, '\0', 256);
    memset(client->data_length_str, '\0', 16);
}

/*
 * Free dynamically allocated memory in the client struct, then free the client
 * struct, itself.
 */
void free_client(struct Client *client) {
    free(client->token);
    free(client->buffer);
    free(client->data_length_str);
    free(client->address);
    free(client);
}

/*
 * Set up the address struct for the server socket.
 */
void init_address(struct sockaddr_in *address, int port) {
    memset((char *) address, '\0', sizeof(*address)); 
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = INADDR_ANY;
}

/*
 * Run the server at the specified port.
 */
void run_server(int port) {
    struct Server *server = malloc(sizeof(struct Server));
    
    init_server(server, port);

    server->socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server->socket < 0) {
        error("Error: socket could not be opened\n", 2);
    } else {
        init_address(server->address, server->port);
    }

    if (bind(server->socket, (struct sockaddr *) server->address,\
    server->size) < 0) {
        error("Error: could not assign a name to the socket\n", 2);
    } else {
        listen(server->socket, server->MAX_CONNECTIONS); 
    }

    while (server->is_running) {
        struct Client *client = malloc(sizeof(struct Client));

        init_client(client);

        client->socket = accept(server->socket,\
        (struct sockaddr *) client->address, &client->size);

        if (client->socket < 0) {
            error("Error: could not accept the connection request\n", -1);
        } else {
            client->recv_length = recv(client->socket, client->token, 1, 0);

            if (client->recv_length < 0) {
                error("Error: could not read from socket\n", -1);
            }

            if (*(client->token) == 'e') {
                *(client->token) = '1';

                send(client->socket, client->token, 1, 0);

                pid_t pid = fork();

                switch (pid) {
                    case -1:
                        error("Error: fork failed", 2);
                        break;
                    case 0:
                        process_request(client);
                        close(client->socket);
                        free_client(client);
                        break;
                    default:
                        break;
                }
            } else {
                *(client->token) = '0';

                send(client->socket, client->token, 1, 0);

                free_client(client);
            }
        }
    }

    close(server->socket);

    free_server(server);
}

/*
 * Process a client request to the server.
 *
 * The first receive is always the length of the incoming data.
 */
void process_request(struct Client *client) {
    printf("Server: connected to client running at host %d port %d\n",\
    ntohs(client->address->sin_addr.s_addr),\
    ntohs(client->address->sin_port));

    client->recv_length = recv(client->socket, client->data_length_str, 16, 0);

    if (client->recv_length < 0) {
        error("Error: could not read from socket\n", -1);
    }

    client->data_length = string_to_integer(client->data_length_str) * 2;

    client->buffer = realloc(client->buffer, client->data_length + 1);

    receive_data(client->socket, client->buffer, client->data_length);

    encrypt_data(client);

    send_data(client->socket, client->buffer, client->data_length / 2);
}

/*
 * Encrypt the data in the client buffer using the passed key.
 */
void encrypt_data(struct Client *client) {
    int length = client->data_length / 2;
    int data_ch = 0;
    int key_ch = 0;
    int enc_ch = 0;
    for (int i = 0; i < length; i++) {
        data_ch = *(client->buffer + i);
        key_ch = *(client->buffer + length + i);

        if (data_ch == 32) {
            data_ch = 91;
        }

        if (key_ch == 32) {
            key_ch = 91;
        }

        data_ch -= 65;
        key_ch -= 65;
        enc_ch = ((data_ch + key_ch) % 27) + 65;

        if (enc_ch == 91) {
            enc_ch = 32;
        }

        *(client->buffer + i) = enc_ch;
    }
}