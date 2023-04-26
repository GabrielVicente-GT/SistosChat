#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include "chat.pb.h"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hola desde el servidor!";

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Fallo al crear socket" << std::endl;
        return -1;
    }

    // Configurar opciones de socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Fallo al configurar opciones de socket" << std::endl;
        return -1;
    }

    // Configurar estructura de dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Asociar socket a dirección y puerto
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::cerr << "Fallo al asociar socket a dirección y puerto" << std::endl;
        return -1;
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Fallo al escuchar conexiones entrantes" << std::endl;
        return -1;
    }

    // Aceptar conexión entrante y enviar mensaje al cliente
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
        std::cerr << "Fallo al aceptar conexión entrante" << std::endl;
        return -1;
    }

    // Crear y enviar mensaje de bienvenida al cliente
    chat::Message welcome_message;
    welcome_message.set_receiver("cliente");
    welcome_message.set_sender("servidor");
    welcome_message.set_message("¡Bienvenido al chat!");
    std::string serialized_message = welcome_message.SerializeAsString();
    send(new_socket, serialized_message.c_str(), serialized_message.length(), 0);
    std::cout << "Mensaje de bienvenida enviado al cliente" << std::endl;

    // Ciclo infinito para leer mensajes del cliente y del servidor
    while (true) {
        // Leer mensaje enviado por el cliente
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        chat::Message client_message;
        if (!client_message.ParseFromArray(buffer, valread)) {
            std::cerr << "Fallo al parsear mensaje del cliente" << std::endl;
            continue;
        }
        std::cout << "Mensaje recibido del cliente:" << std::endl;
        std::cout << "  Receptor: " << client_message.receiver() << std::endl;
        std::cout << "  Remitente: " << client_message.sender() << std::endl;
        std::cout << "  Mensaje: " << client_message.message() << std::endl;

        // Verificar si se recibió el mensaje de terminar
        if (client_message.message() == "Terminar") {
            break;
        }

        // Leer mensaje enviado desde la terminal
        std::string message_receiver, message_sender, message_text;
        std::cout << "Escriba un mensaje para enviar al cliente:" << std::endl;
        std::cout << "  Receptor: ";
        std::getline(std::cin, message_receiver);
        std::cout << "  Remitente: ";
        std::getline(std::cin, message_sender);
        std::cout << "  Mensaje: ";
        std::getline(std::cin, message_text);

        // Crear y enviar mensaje al cliente
        chat::Message server_message;
        server_message.set_receiver(message_receiver);
        server_message.set_sender(message_sender);
        server_message.set_message(message_text);
        std::string serialized_message = server_message.SerializeAsString();
        send(new_socket, serialized_message.c_str(), serialized_message.length(), 0);
        std::cout << "Mensaje enviado al cliente" << std::endl;
    }
    
    // Cerrar socket
    close(new_socket);
    close(server_fd);
    return 0;
}