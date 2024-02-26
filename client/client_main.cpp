#include <map>
#include "client.hpp"

std::string server_address("127.0.0.1:12345");

enum class Commands { LOGIN, SIGNIN, INVALID };
std::map<std::string, Commands> commands = {
    {"Log in", Commands::LOGIN},
    {"Sign in", Commands::SIGNIN},
};

Commands parse_command(std::string command) {
    if (commands.find(command) == commands.end()) {
        return Commands::INVALID;
    }
    return commands[command];
}

std::string read_field(std::string field) {
    std::string input;
    std::cout << "Enter " << field << ": ";
    std::getline(std::cin, input);
    return input;
}

void execute(Commands command, client &new_client) {
    switch (command) {
        case Commands::LOGIN: {
            std::string username = read_field("username");
            std::string password = read_field("password");
            if (new_client.login(username, password).ok()) {
                std::cout << "Login successful. Welcome back!" << std::endl;
            }
            break;
        }
        case Commands::SIGNIN: {
            std::string username = read_field("username");
            std::string email = read_field("email");
            std::string password = read_field("password");
            std::string password_confirmation = read_field("password confirmation");
            if (new_client.registration(username, email, password, password_confirmation).ok()) {
                std::cout << "Signin successful. Welcome!" << std::endl;
            }
            break;
        }
        case Commands::INVALID: {
            std::cout << "Invalid command" << std::endl;
            break;
        }
        default: {
            break;
        }
    }
}

int main() {
    client new_client(
        grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials())
    );

    std::string input;

    while (true) {
        std::cout << "Choose: Log in or Sign in?\n";
        std::getline(std::cin, input);
        Commands command = parse_command(input);
        execute(command, new_client);
    }

    return 0;
}
