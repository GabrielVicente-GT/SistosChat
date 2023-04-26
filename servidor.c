#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "chat.pb-c.h"

#define BACKLOG 10
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    // Recibir un buffer del socket
    uint8_t recv_buffer[BUFFER_SIZE];
    ssize_t recv_size = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0);
    if (recv_size < 0) {
        perror("Error al recibir el mensaje");
        exit(1);
    }

    // Deserializar el buffer en un mensaje Message
    Chat__Message *message = chat__message__unpack(NULL, recv_size, recv_buffer);

    printf("Mensaje recibido de %s: %s\n", message->sender, message->message);

    // Crear una respuesta
    Chat__Message response = CHAT__MESSAGE__INIT;
    response.sender = "servidor";
    response.receiver = message->sender;
    response.message = "Mensaje recibido correctamente";

    // Serializar la respuesta en un buffer
    size_t serialized_size = chat__message__get_packed_size(&response);
    uint8_t *buffer = malloc(serialized_size);
    chat__message__pack(&response, buffer);

    // Enviar el buffer de respuesta a través del socket
    if (send(client_socket, buffer, serialized_size, 0) < 0) {
        perror("Error al enviar la respuesta");
        exit(1);
    }

    // Liberar los buffers y el mensaje
    free(buffer);
    chat__message__free_unpacked(message, NULL);
    close(client_socket);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Uso: %s <puerto>\n", argv[0]);
        exit(1);
    }

    int server_port = atoi(argv[1]);

    // Crear el socket del servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error al crear el socket del servidor");
        exit(1);
    }

    // Permitir la reutilización de la dirección y puerto del servidor
    int option = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        perror("Error al configurar las opciones del socket");
        exit(1);
    }

    // Configurar la dirección y puerto del servidor
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    // Enlazar el socket del servidor a la dirección y puerto especificados
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error al enlazar el socket del servidor");
        exit(1);
    }

    // Escuchar conexiones entrantes
    if (listen(server_socket, BACKLOG) < 0) {
        perror("Error al escuchar conexiones entrantes");
        exit(1);
    }

    printf("Servidor iniciado en el puerto %d\n", server_port);

    while (1) {
        // Aceptar una conexión
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
        perror("Error al aceptar la conexión entrante");
        exit(1);
    }

    printf("Nueva conexión entrante desde %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    handle_client(client_socket);
}

return 0;
}