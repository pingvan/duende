protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc=/home/kolesovasonya/hse/duende/grpc/cmake/build/grpc_cpp_plugin authservice.proto

protoc --cpp_out=. authservice.proto