#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include "chat.pb-c.h"

#define BACKLOG 10
#define BUFFER_SIZE 1024

typedef  struct {
    char username[100];
    char ip[50];
    int socketFD;
    int status;
} User;


#define MAX_USERS 50 // Define el tamaño máximo de la lista

User userList[MAX_USERS]; // Crea un arreglo de estructuras de usuario con tamaño máximo de MAX_USERS
int numUsers = 0; // El número actual de usuarios en la lista, inicializado en 0

// Agrega un usuario a la lista
void addUser(char* username, char* ip, int socketFD, int status) {
    if (numUsers >= MAX_USERS) {
        printf("La lista de usuarios está llena. No se puede agregar más usuarios.\n");
        return;
    }
    User newUser;
    strcpy(newUser.username, username);
    strcpy(newUser.ip, ip);
    newUser.socketFD = socketFD;
    newUser.status = status;
    userList[numUsers] = newUser;
    numUsers++;
}

// Elimina un usuario de la lista
void removeUser(char* username, char* ip, int socketFD, int status) {
    int i, j;
    for (i = 0; i < numUsers; i++) {
        User user = userList[i];
        if (strcmp(user.username, username) == 0 && strcmp(user.ip, ip) == 0 && user.socketFD == socketFD && user.status == status) {
            // Encontró el usuario, lo elimina
            for (j = i; j < numUsers-1; j++) {
                userList[j] = userList[j+1];
            }
            numUsers--;
            printf("Usuario eliminado: %s\n", username);
            return;
        }
    }
    // No encontró al usuario
    printf("No se encontró al usuario: %s\n", username);
}


// void handle_client(int client_socket) {
//     // Recibir un buffer del socket
//     uint8_t recv_buffer[BUFFER_SIZE];
//     ssize_t recv_size = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0);
//     if (recv_size < 0) {
//         perror("Error al recibir el mensaje");
//         exit(1);
//     }

//     // Deserializar el buffer en un mensaje Message
//     Chat__Message *message = chat__message__unpack(NULL, recv_size, recv_buffer);

//     printf("Mensaje recibido de %s: %s\n", message->message_sender, message-> message_content);

//     // Crear una respuesta
//     Chat__Message response          = CHAT__MESSAGE__INIT;
//     response.message_private        = '0';
//     response.message_destination    = "Cliente";
//     response.message_content        = "Mensaje recibido correctamente";
//     response.message_sender = message->message_sender;

//     // Serializar la respuesta en un buffer
//     size_t serialized_size = chat__message__get_packed_size(&response);
//     uint8_t *buffer = malloc(serialized_size);
//     chat__message__pack(&response, buffer);

//     // Enviar el buffer de respuesta a través del socket
//     if (send(client_socket, buffer, serialized_size, 0) < 0) {
//         perror("Error al enviar la respuesta");
//         exit(1);
//     }

//     // Liberar los buffers y el mensaje
//     free(buffer);
//     chat__message__free_unpacked(message, NULL);
//     close(client_socket);
// }
void* handle_client(void *arg) {
    int client_socket = *(int*)arg;

    // Recibir el registro del cliente
    uint8_t recv_buffer[BUFFER_SIZE];
    ssize_t recv_size = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0);
    if (recv_size < 0) {
        perror("Error al recibir el mensaje del cliente");
        exit(1);
    }

    // Deserializar el registro de NewUser
    Chat__NewUser *chat_registration = chat__new_user__unpack(NULL, recv_size, recv_buffer);
    if (chat_registration == NULL) {
        fprintf(stderr, "Error al deserializar el mensaje del cliente\n");
        exit(1);
    }

    printf("\n >> Nuevo usuario conectado! \n >> Nombre: %s \n >> IP: %s\n", chat_registration->username, chat_registration-> ip);
    // printf("Mensaje recibido del cliente %d: %s\n", client_socket, chat_message->content);

    // Agregar usuario conectado a la lista de usuarios
    addUser(chat_registration->username,chat_registration->ip,client_socket, 0);

    // Informacion del Cliente asociada al thread
    User MyInfo;
    strcpy(MyInfo.username, chat_registration->username);
    strcpy(MyInfo.ip, chat_registration->ip);
    MyInfo.socketFD = client_socket;
    MyInfo.status = 0;
    printf("Nombre de usuario: %s\n", MyInfo.username);
    printf("Dirección IP: %s\n", MyInfo.ip);
    printf("Descriptor de archivo del socket: %d\n", MyInfo.socketFD);
    printf("Estado: %d\n", MyInfo.status);


    // Respuesta del servidor
    Chat__Message response          = CHAT__MESSAGE__INIT;
    response.message_private        = '1';
    response.message_destination    = MyInfo.username;
    response.message_content        = "Fuiste registrado!";
    response.message_sender         = "Servidor";

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
    // Liberar los recursos utilizados por el mensaje y cerrar el socket del cliente
    chat__new_user__free_unpacked(chat_registration, NULL);

    printf("\n\n ---- Usuarios dentro del chat ----\n");

    for (int i = 0; i < numUsers; i++) {
        printf("Información del usuario #%d:\n", i+1);
        printf("Nombre de usuario: %s\n", userList[i].username);
        printf("Dirección IP: %s\n", userList[i].ip);
        printf("Descriptor de archivo del socket: %d\n", userList[i].socketFD);
        printf("Estado: %d\n", userList[i].status);
        printf("\n");
    }

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
        // Esperar a que llegue una conexión
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Error al aceptar la conexión del cliente");
            exit(1);
        }

        printf("\nCliente conectado desde %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Crear un nuevo hilo para el cliente
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_socket) < 0) {
            perror("Error al crear el hilo del cliente");
            exit(1);
        }
    }


return 0;

}