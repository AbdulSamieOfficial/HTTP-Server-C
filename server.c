#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 10

char buffer[BUFFER_SIZE];
char response_header[BUFFER_SIZE]; 
char response_body[BUFFER_SIZE];
int header_size;
int body_size;

char written_data[BUFFER_SIZE];
bool data_written = false;

// Stats variables
int requests = 0;
int header_bytes = 0;
int body_bytes = 0;
int errors = 0;
int error_bytes = 0;

void send_response(int client_socket) {
    write(client_socket, response_header, header_size);
    write(client_socket, "\r\n", 2);
    write(client_socket, response_body, body_size);
    write(client_socket, "\r\n", 2);
}

void handle_ping(int client_socket) {
    header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: 4");
    body_size = snprintf(response_body, BUFFER_SIZE, "pong");
    send_response(client_socket);

    // Update stats
    requests++;
    header_bytes += header_size;
    body_bytes += body_size;
}

void handle_echo(int client_socket, char *request_headers) {
    header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: %zu", strlen(request_headers));
    body_size = snprintf(response_body, BUFFER_SIZE, "%s", request_headers);
    send_response(client_socket);

    // Update stats
    requests++;
    header_bytes += header_size;
    body_bytes += body_size;
}

void handle_write(int client_socket, char *request_headers, char *request_body) {
    // Find the Content-Length header value
    char *content_length_header = strstr(request_headers, "Content-Length: ");
    if (!content_length_header) {
        header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 400 Bad Request\r\n");
        send_response(client_socket);
        errors++;
        error_bytes += header_size;
        return;
    }
    int content_length = atoi(content_length_header + 16);

    // Save the data to the written_data buffer
    strncpy(written_data, request_body, content_length);
    data_written = true;

    header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: %d", content_length);
    body_size = snprintf(response_body, BUFFER_SIZE, "%s", written_data);
    send_response(client_socket);

    // Update stats
    requests++;
    header_bytes += header_size;
    body_bytes += body_size;
}

void handle_read(int client_socket) {
    if (!data_written) {
        header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: 7");
        body_size = snprintf(response_body, BUFFER_SIZE, "<empty>");
    } else {
        header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: %zu", strlen(written_data));
        body_size = snprintf(response_body, BUFFER_SIZE, "%s", written_data);
    }
    send_response(client_socket); // Added the closing parenthesis here

    // Update stats
    requests++;
    header_bytes += header_size;
    body_bytes += body_size;
}

void handle_stats(int client_socket) {
    char stats_content[BUFFER_SIZE];
    int content_length = snprintf(stats_content, BUFFER_SIZE, "Requests: %d\nHeader bytes: %d\nBody bytes: %d\nErrors: %d\nError bytes: %d", requests, header_bytes, body_bytes, errors, error_bytes);

    header_size = snprintf(response_header, BUFFER_SIZE, "HTTP/1.1 200 OK\r\nContent-Length: %d", content_length);
    body_size = snprintf(response_body, BUFFER_SIZE, "%s", stats_content);
    send_response(client_socket);

    // Update stats
    requests++;
    header_bytes += header_size;
    body_bytes += body_size;
}

void *client_handler(void *client_socket_ptr) {
    int client_socket = *((int *)client_socket_ptr);
    free(client_socket_ptr);

    // Read request from client
    read(client_socket, buffer, BUFFER_SIZE);

    // Parse request
    char *request_line = strtok(buffer, "\r\n");
    char *request_headers = strtok(NULL, "\r\n\r\n");
    char *request_body = request_headers ? strtok(NULL, "") : NULL;

    // Handle request
    if (strncmp(request_line, "GET /ping", 9) == 0) {
        handle_ping(client_socket);
    } else if (strncmp(request_line, "GET /echo", 9) == 0) {
        handle_echo(client_socket, request_headers);
    } else if (strncmp(request_line, "POST /write", 11) == 0) {
        handle_write(client_socket, request_headers, request_body);
    } else if (strncmp(request_line, "GET /read", 9) == 0) {
        handle_read(client_socket);
    } else if (strncmp(request_line, "GET /stats", 10) == 0) {
        handle_stats(client_socket);
    } else {
        
    }

    // Close connection
    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, BACKLOG);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;
        pthread_t client_thread;
        pthread_create(&client_thread, NULL, client_handler, client_socket_ptr);
        pthread_detach(client_thread);
    }

    close(server_socket);
    return 0;
}
