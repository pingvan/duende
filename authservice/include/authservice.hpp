#ifndef AUTHORIZATION_HPP_
#define AUTHORIZATION_HPP_

#include <grpcpp/grpcpp.h>


namespace AuthService {
    
    struct login_user_does_not_exist : public std::exception {
        const char *what() const noexcept override {
            return "User does not exist";
        }
    };
    struct login_invalid_password : public std::exception {
        const char *what() const noexcept override {
            return "Invalid password";
        }
    };
    struct signin_user_is_already_exist : public std::exception {
        const char *what() const noexcept override {
            return "User is already exist";
        }
    };
    struct signin_invalid_username_format : public std::exception {
        const char *what() const noexcept override {
            return "Invalid username format";
        }
    };
    struct signin_invalid_email_format : public std::exception {
        const char *what() const noexcept override {
            return "Invalid email format";
        }
    };
    struct signin_invalid_password_format : public std::exception {
        const char *what() const noexcept override {
            return "Invalid password format";
        }
    };

    bool signin_check_username(std::string username);
    bool signin_check_email(std::string email);
    bool signin_check_password(std::string password);

    std::vector <std::string> parsed_input(std::string input);

    grpc::Status Login(std::string user, std::string password);
    grpc::Status Signin(std::string user, std::string email, std::string password);
}

#endif
