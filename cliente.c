#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <curl/curl.h>
#include <ctype.h>

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

void print_menu() {
    printf("\nMenu:\n");
    printf("1. Chatear con otros (general)\n");
    printf("2. Mensaje privado\n");
    printf("3. Cambiar status\n");
    printf("4. Mostrar todos los usuarios conectados\n");
    printf("5. Desplegar informacion particular de un usuario\n");
    printf("6. Ayuda\n");
    printf("7. Salir\n");
    printf("Enter your choice: "); 
}

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

    printf("\nConectado al servidor %s:%d\n\n", server_ip, server_port);

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
    free(buffer);
    free(ip);
    printf(" >> Registro enviado!");

    /*Respuesta del servidor*/
    
    // Recibir un buffer del socket
    uint8_t recv_buffer[BUFFER_SIZE];
    ssize_t recv_size = recv(client_socket, recv_buffer, sizeof(recv_buffer), 0);
    if (recv_size < 0) {
        perror("Error al recibir la respuesta");
        exit(1);
    }

    // Deserializar el buffer en un mensaje Message
    Chat__Message *response = chat__message__unpack(NULL, recv_size, recv_buffer);

    printf("\n\n ---- Respuesta de registro ----\n");
    printf(" >> TIPO DE MENSAJE: %d\n",     response->message_private);
    printf(" >> SENDER: %s\n",              response->message_sender);
    printf(" >> DESTINATION: %s\n",         response->message_destination);
    printf(" >> CONTENT: %s\n\n",             response->message_content);
    
    // printf("\n\n [%s] --> [%s]: '%s' \n\n", response->message_sender, response->message_destination, response->message_content);

    // Liberar los buffers y el mensaje
    chat__message__free_unpacked(response, NULL);

    /*Menu para el usuario*/
    int choice = 0;
    while(choice != 7) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    // Chat with others
                    char private[] = "0"; // Public message indicator
                    char destination[] = "";
                    char content[BUFFER_SIZE];

                    // ingresar los datos
                    printf("Esriba el mensaje: ");
                    scanf(" %[^\n]", content); // Leer hasta que se le agreguen datos
                    
                    printf("\nprivate: %s", private);
                    printf("\ndestination: %s", destination);
                    printf("\ncontent: %s", content);
                    printf("\nsender: %s", username);
                }
                break;
            case 2:
                // Private message
                {
                    // Chat with others
                    char private[] = "1"; // Public message indicator
                    char destination[BUFFER_SIZE];
                    char content[BUFFER_SIZE];

                    // ingresar los datos
                    printf("Ingresar destinatario: ");
                    scanf(" %[^\n]", destination);  // Leer hasta que se le agreguen datos
                    printf("Esriba el mensaje: ");
                    scanf(" %[^\n]", content); // Leer hasta que se le agreguen datos
                    
                    printf("\nprivate: %s", private);
                    printf("\ndestination: %s", destination);
                    printf("\ncontent: %s", content);
                    printf("\nsender: %s", username);
                }
                break;
            case 3:
                {
                    // Change status
                    char input_str;
                    int state;
                    int valid_input = 0;
                    while (!valid_input) {
                        valid_input = 1;

                        printf("Choose an option:\n");
                        printf("1. En linea\n");
                        printf("2. Ocuapdo\n");
                        printf("3. Desconectado\n");
                        scanf(" %c", &input_str); // Read a char from input, skipping whitespace

                        // Check if the input is a valid digit (1 to 3)
                        if (isdigit(input_str) && input_str >= '1' && input_str <= '3') {
                            state = input_str - '0'; // Convert the character to its corresponding integer value
                        } else {
                            printf("Invalid option. Please choose a number between 1 and 3.\n");
                            valid_input = 0; // Set valid_input to false
                        }

                    }

                    printf("\nstate: %i", state);
                    printf("\nsender: %s", username);
                }
                break;
            case 4:
                // Show all connected users
                {
                    char connectedUsers = 0;
                }
                break;
            case 5:
                // Show specific user information
                {
                    char UserInformation[BUFFER_SIZE];

                    // ingresar los datos
                    printf("Ingresar nombre de usuario que desea obtener la informacion: ");
                    scanf(" %[^\n]", UserInformation);  // Leer hasta que se le agreguen datos
                
                    printf("UserInformation: %s", UserInformation);
                }
                break;
            case 6:
                // Help
                {
                    char help = 0;
                }
                break;
            case 7:
                // Exit
                printf("Exiting...");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    close(client_socket);

    return 0;
}
