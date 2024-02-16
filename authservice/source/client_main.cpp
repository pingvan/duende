#include "client.hpp"

std::string server_address("127.0.0.1:12345");

int main() {
    client new_client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
    new_client.login("test", "testpwd");
    return 0;
}
