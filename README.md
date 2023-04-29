# SistosChat

protoc --c_out=. chat.proto


gcc -o servidor servidor.c chat.pb-c.c -pthread -lprotobuf-c

./servidor 8080


gcc -o cliente cliente.c chat.pb-c.c -lprotobuf-c

./cliente 127.0.0.1 8080 usuario1

status

0 -> ocupado
1 -> inactivo
2 -> activo