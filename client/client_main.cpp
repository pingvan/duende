#include <unistd.h>
#include <map>
#include <filesystem>
#include <fstream>
#include "client.hpp"

std::string server_address("127.0.0.1:12345");

void read(const std::string &path, std::string& content) {
    // std::cout << "Reading file: " << path << "\n";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << '\n';
        std::exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        // std::cout << line << '\n';
        content += line + '\n';
    }
    // std::cout << "File read successfully\n";
}

int main() {
    std::string key;
    std::string cert;
    std::string root;

    auto key_path = std::filesystem::path("cert/client.key");
    auto cert_path = std::filesystem::path("cert/client.crt");
    auto root_path = std::filesystem::path("cert/ca.crt");

    auto current_path = std::filesystem::current_path();
    auto parent_path = current_path.parent_path();

    read(parent_path / key_path, key);
    read(parent_path / cert_path, cert);
    read(parent_path / root_path, root);

    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = root;
    // ssl_opts.pem_private_key = "";

    auto channel_credentials = grpc::SslCredentials(ssl_opts);

    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    client new_client(
        grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials())
    );

    return 0;
}
