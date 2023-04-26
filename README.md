# SistosChat

g++ server.cpp chat.pb.cc -pthread -lprotobuf -o server

g++ client.cpp chat.pb.cc -pthread -lprotobuf -o client

protoc chat.proto --cpp_out=.
