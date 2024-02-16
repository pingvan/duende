#include "authservice.hpp"
#include "data.hpp"
#include <exception>

grpc::Status AuthService::Login(std::string username, std::string password) {
    User supposed_user(username, password);
    for (auto &user : users) {
        if (user.GetUsername() == supposed_user.GetUsername()) {
            if (user.GetPassword() == supposed_user.GetPassword()) {
                return grpc::Status::OK;
            }
            throw login_invalid_password();
        }
    }
    throw login_user_does_not_exist();
    return grpc::Status::CANCELLED;
}

grpc::Status AuthService::Signin(std::string username, std::string email, std::string password) {
    User supposed_user(username, email, password);
    if (!signin_check_username(username)) {
        throw signin_invalid_username_format();
    }
    if (!signin_check_email(email)) {
        throw signin_invalid_email_format();
    }
    if (!signin_check_password(password)) {
        throw signin_invalid_password_format();
    }

    for (auto &user : users) {
        if (user == supposed_user) {
            throw signin_user_is_already_exist();
            return grpc::Status::CANCELLED;
        }
    }
    users.push_back(supposed_user);
    return grpc::Status::OK;
}

bool AuthService::signin_check_username(std::string username) {
    std::vector <std::string> parsed = parsed_input(username);
    if (parsed.size() != 2) {
        return false;
    }
    for (auto &name : parsed) {
        for (int i = 0; i < name.size(); i++) {
            if (i == 0 && !std::isupper(name[i])) {
                return false;
            }
            if (i != 0 && !std::islower(name[i])) {
                return false;
            }
            if (!std::isalpha(name[i])) {
                return false;
            }
        }
    }
    return true;
}

bool AuthService::signin_check_email(std::string email) {
    if (email.empty()) {
        return false;
    }
    for (auto &symbol : email) {
        if (!std::isalnum(symbol) && symbol != '@' && symbol != '.') {
            return false;
        }
    }
    if (email.find('@') == std::string::npos || email.find('.') == std::string::npos || email.find('@') > email.find('.')) {
        return false;
    }
    return true;
}

bool AuthService::signin_check_password(std::string password) {
    if (password.empty()) {
        return false;
    }   
    int upper = 0, lower = 0, digit = 0;
    for (auto &symbol : password) {
        if (std::isspace(symbol)) {
            return false;
        }
        if (std::isupper(symbol)) {
            upper++;
        }
        if (std::islower(symbol)) {
            lower++;
        }
        if (std::isdigit(symbol)) {
            digit++;
        }
    }
    return upper > 0 && lower > 0 && digit > 0 && password.size() > 7;
}

std::vector <std::string> AuthService::parsed_input(std::string input) {
    if (input.empty()) {
        return {};
    }
    std::vector <std::string> parsed;
    std::string temp;
    for (auto &symbol : input) {
        if (std::isspace(symbol) != 0 && !temp.empty()) {
            parsed.push_back(temp);
            temp.clear();
        } else {
            temp += symbol;
        }
    }
    if (!temp.empty()) {
        parsed.push_back(temp);
    }
    return parsed;
}
