#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <curl/curl.h>

#include "chat.pb-c.h"

#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h> 

#define BUFFER_SIZE 1024

// size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
// {
//     size_t response_size = size * nmemb;
//     char* response = (char*)userp;
//     memcpy(response, contents, response_size);
//     return response_size;
// }

char *get_local_ip()
{
    struct ifaddrs *ifaddr, *ifa;
    char *local_ip = NULL;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        int family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            local_ip = malloc(INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, local_ip, INET_ADDRSTRLEN);
            break;
        }
    }

    freeifaddrs(ifaddr);
    return local_ip;
}

int main(int argc, char **argv) {
    
    if (argc != 4) {
        printf("Uso: %s <ip_servidor> <puerto_servidor> <usuario>\n", argv[0]);
        exit(1);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    char *username = argv[3];

    // Crear el socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket");
        exit(1);
    }

    // Conectar al servidor
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_address.sin_addr);



    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error al conectar con el servidor");
        exit(1);
    }

    printf("Conectado al servidor %s:%d\n", server_ip, server_port);

    // Si se conecto manda el registro
    Chat__NewUser registration = CHAT__NEW_USER__INIT;
    registration.username    = username;
    char *ip = get_local_ip();
    registration.ip = ip;

    // Serializando registro
    size_t serialized_size = chat__new_user__get_packed_size(&registration);
    uint8_t *buffer = malloc(serialized_size);
    chat__new_user__pack(&registration, buffer);

    // Enviar registro
    if (send(client_socket, buffer, serialized_size, 0) < 0) {
        perror("Error al enviar el mensaje");
        exit(1);
    }

    printf("Mensaje enviado: %s\n", registration.ip);

    /*
    Respuesta del servidor
    */


    // Recibir un buffer del socket
    uint8_t recv_buffer[BUFFER_SIZE];
    ssize_t recv_size = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0);
    if (recv_size < 0) {
        perror("Error al recibir la respuesta");
        exit(1);
    }

    // Deserializar el buffer en un mensaje Message
    Chat__Message *response = chat__message__unpack(NULL, recv_size, recv_buffer);

    printf("Respuesta recibida: %s\n", response->message_content);

    // Liberar los buffers y el mensaje
    free(buffer);
    free(ip);
    chat__message__free_unpacked(response, NULL);
    close(client_socket);

    return 0;
}
