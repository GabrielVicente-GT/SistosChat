
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include "chat.pb.h"

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char *terminar = "Terminar";
    bool terminado = false;

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Fallo al crear socket" << std::endl;
        return -1;
    }

    // Configurar estructura de dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convertir dirección IP de texto a binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Dirección inválida o no soportada" << std::endl;
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Fallo al conectar al servidor" << std::endl;
        return -1;
    }
    while (!terminado) {
        // Leer mensaje enviado por el servidor
        chat::Message mensaje_recibido;
        memset(buffer, 0, sizeof(buffer));
        valread = read(sock, buffer, 1024);
        mensaje_recibido.ParseFromArray(buffer, valread);
        // std::cout << "Mensaje recibido del servidor: " << mensaje_recibido.message() << std::endl;
        std::cout << "Mensaje recibido del Servidor:" << std::endl;
        std::cout << "  Receptor: " << mensaje_recibido.receiver() << std::endl;
        std::cout << "  Remitente: " << mensaje_recibido.sender() << std::endl;
        std::cout << "  Mensaje: " << mensaje_recibido.message() << std::endl;
        // Si el mensaje recibido es "Terminar", salir del ciclo
        if (mensaje_recibido.message() == terminar) {
            terminado = true;
            continue;
        }

        // Si el mensaje recibido es el mensaje inicial del servidor, imprimirlo y seguir esperando por el próximo mensaje
        if (mensaje_recibido.message() == "Hola desde el servidor") {
            std::cout << mensaje_recibido.message() << std::endl;
            continue;
        }

        // Pedir al usuario que escriba un mensaje
        std::string mensaje;
        std::cout << "Escriba un mensaje (Terminar para salir): ";
        std::getline(std::cin, mensaje);

        // Crear y enviar mensaje al servidor
        chat::Message mensaje_enviado;
        mensaje_enviado.set_sender("Cliente");
        mensaje_enviado.set_message(mensaje);
        send(sock, mensaje_enviado.SerializeAsString().c_str(), mensaje_enviado.SerializeAsString().length(), 0);
        std::cout << "Mensaje enviado al servidor" << std::endl;

        // Si el mensaje enviado es "Terminar", salir del ciclo
        if (mensaje == terminar) {
            terminado = true;
        }
    }

    // Cerrar socket
    close(sock);
    return 0;
}
