#include "dec_client.h"

/*
 * Run the client, sending the plaintext and key to the specified port.
 */
int main(int argc, char **argv) {
    if (argc < 4) { 
        error("Error: expected three arguments, plaintext, key, and port\n", 2);
    } else {
        run_client(*(argv + 1), *(argv + 2), string_to_integer(*(argv + 3)));
    }
}

/*
 * Set up the address struct.
 */
void init_address(struct sockaddr_in* address, int port, char* hostname) {
    struct hostent* host_info = gethostbyname(hostname); 
    if (host_info == NULL) { 
        error("Error: no such host\n", 2);
    } else {
        memset((char*) address, '\0', sizeof(*address)); 
        address->sin_family = AF_INET;
        address->sin_port = htons(port);
        memcpy((char*) &address->sin_addr.s_addr, host_info->h_addr_list[0],\
        host_info->h_length);
    }
}

/*
 * Initialize the client struct.
 */
void init_client(struct Client *client, int port) {
    client->port = port;
    client->socket = -1;
    client->data_length = 0;
    client->key_length = 0;
    client->index = 0;
    client->buffer = malloc(sizeof(char) * 256);
    client->data_length_str = malloc(sizeof(char) * 16);
    client->server_address = malloc(sizeof(struct sockaddr_in));
    client->server_size = sizeof(*(client->server_address));
    memset(client->buffer, '\0', 256);
    memset(client->data_length_str, '\0', 16);
}

/*
 * Free dynamically allocated memory in the client struct, then free the client
 * struct, itself.
 */
void free_client(struct Client *client) {
    free(client->buffer);
    free(client->data_length_str);
    free(client->server_address);
    free(client);
}

/*
 * Connect to the server, then send the data length, data, and key.
 *
 * The encrypted data will be sent back, then written to stdout.
 */
void run_client(char *plaintext, char *key, int port) {
    struct Client *client = malloc(sizeof(struct Client));

    init_client(client, port);

    client->socket = socket(AF_INET, SOCK_STREAM, 0); 

    if (client->socket < 0){
        error("Error: socket could not be opened\n", 2);
    } else {
        init_address(client->server_address, client->port, "localhost");
    }

    if (connect(client->socket, (struct sockaddr *) client->server_address,\
    client->server_size) < 0) {
        error("Error: could not connect to server\n", 2);
    }

    read_data(client, plaintext);
    read_key(client, key);

    integer_to_string(client->data_length, client->data_length_str);

    send_data(client->socket, client->data_length_str, 16);
    send_data(client->socket, client->buffer, string_length(client->buffer));

    receive_data(client->socket, client->buffer, client->data_length);

    write(1, client->buffer, client->data_length);
    write(1, "\n", 1);

    close(client->socket);

    free_client(client);
}

/*
 * Read the data file.
 *
 * Keep track of the number of characters in the file, while storing the
 * characters in the buffer.
 * 
 * If an illegal character is passed, exit the program.
 */
void read_data(struct Client *client, char *filename) {
    int is_reading = 1;
    int count = 0;
    char ch = '\0';
    FILE *fp;

    fp = fopen(filename, "r");

    while (is_reading) {
        ch = getc(fp);
        if ((ch >= 65 && ch <= 90) || ch == 32) {
            client->data_length += 1;
            client->index += 1;
            *(client->buffer + client->index - 1) = ch;
            if (count < client->index) {
                count = client->index * 2;
                client->buffer = reallocate_string(client->buffer,\
                client->index, count);
            }
        } else if (ch == '\0' || ch == EOF || ch == '\n') {
            fclose(fp);
            is_reading = 0;
        } else {
            fclose(fp);
            error("Error: erroneous data detected\n", 2);
        }
    }

    client->buffer = reallocate_string(client->buffer, client->index, 1);
}

/*
 * Read the key file.
 *
 * Append the key to the data buffer.
 * 
 * If the key contains illegal characters, or there are less characters in the
 * key than the data, exit the program.
 */
void read_key(struct Client *client, char *filename) {
    int is_reading = 1;
    int is_term = 0;
    int count = 0;
    char ch = '\0';
    FILE *fp;

    fp = fopen(filename, "r");

    while (is_reading) {
        ch = getc(fp);

        if (((ch >= 65 && ch <= 90) || ch == 32) &&\
        client->key_length <= client->data_length) {
            client->key_length += 1;
            client->index += 1;
            *(client->buffer + client->index - 1) = ch;
            if (count < client->index) {
                count = client->index * 2;
                client->buffer = reallocate_string(client->buffer,\
                client->index, count);
            }
        } else if (ch == '\0' || ch == EOF || ch == '\n' || ch =='\r') {
            is_term = 1;
        } else {
            fclose(fp);
            error("Error: erroneous data detected\n", 2);
        }

        if (client->key_length == client->data_length) {
            is_term = 1;
        }

        if (is_term) {
            fclose(fp);
            is_reading = 0;
            if (client->key_length != client->data_length) {
                error("Error: erroneous key file length\n", 2);
            }
        }
    }

    client->buffer = reallocate_string(client->buffer, client->index, 1);
}
