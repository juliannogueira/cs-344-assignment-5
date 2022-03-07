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
    client->BUFFER_SIZE = 10;
    client->socket = -1;
    client->recv_length = 0;
    client->send_length = 0;
    client->index = 0;
    client->message_length_int = 0;
    client->buffer = malloc(sizeof(char) * client->BUFFER_SIZE);
    client->message_length_str = malloc(sizeof(char));
    client->address = malloc(sizeof(struct sockaddr_in));
    client->size = sizeof(*(client->address));
    *(client->message_length_str + 0) = '\0';
}

/*
 * Free dynamically allocated memory in the client struct, then free the client
 * struct, itself.
 */
void free_client(struct Client *client) {
    free(client->buffer);
    free(client->message_length_str);
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
            process_request(client);
            // break;
        } 
    }

    close(server->socket);

    free_server(server);
}

/*
 * Process a client request to the server.
 */
void process_request(struct Client *client) {
    char ch = '\0';

    printf("Server: connected to client running at host %d port %d\n",\
    ntohs(client->address->sin_addr.s_addr),\
    ntohs(client->address->sin_port));

    memset(client->buffer, '\0', client->BUFFER_SIZE);

    client->recv_length = recv(client->socket, client->buffer,\
    client->BUFFER_SIZE, 0); 

    if (client->recv_length < 0) {
        error("Error: could not read from socket\n", -1);
    } else {
        printf("Length: %d\n", client->recv_length);
    }

    for (int i = 0; i < client->recv_length; i++) {
        client->index = i;

        ch = *(client->buffer + i);

        if (ch >= 48 && ch <= 57) {
            client->message_length_str = realloc(client->message_length_str,\
            sizeof(char) * (i + 2));
            *(client->message_length_str + i) = ch;
        } else {
            *(client->message_length_str + i) = '\0';
            break;
        }
    }

    client->message_length_int = string_to_integer(client->message_length_str);

    printf("%s -> %d\n", client->message_length_str,\
    client->message_length_int);

    client->send_length = send(client->socket, client->message_length_str,\
    string_length(client->message_length_str), 0);

    if (client->send_length < 0){
        error("Error: could not write to socket\n", -1);
    }

    close(client->socket);

    free_client(client);
}
