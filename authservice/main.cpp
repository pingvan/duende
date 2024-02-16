#include <iostream>
#include "authservice.hpp"
#include "data.hpp"

int main() {

    std::string command;

    while (std::getline(std::cin, command)) {
        if (command == "Log in") {
            std::string username, password;
            std::getline(std::cin, username);
            std::getline(std::cin, password);
            try {
                if (AuthService::Login(username, password).ok()) {
                    std::cout << "Login successful. Welcome back!" << std::endl;
                    break;
                }
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "Sign in") {
            std::string username, email, password;
            std::getline(std::cin, username);
            std::getline(std::cin, email);
            std::getline(std::cin, password);
            try {
                if (AuthService::Signin(username, email, password).ok()) {
                    std::cout << "Signin successful. Welcome!" << std::endl;
                    std::cout << "Your token: " << jwt_token(username) << std::endl;
                    break;
                }
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }

    return 0;
}
