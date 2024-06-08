#include "service.hpp"

grpc::Status auth_service::password_is_valid(const std::string &password) {
    for (char symbol : password) {
        if (std::isspace(symbol)) {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT,
                "Password must not contain spaces"
            );
        }
    }
    if (password.empty()) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Password must not be empty"
        );
    }
    return grpc::Status::OK;
}

grpc::Status auth_service::username_is_valid(const std::string &username) {
    if (username.empty()) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Username must not be empty"
        );
    }
    if (username.size() > 20) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Username is too long. It must be at most 20 characters long"
        );
    }
    return grpc::Status::OK;
}

grpc::Status auth_service::email_is_valid(const std::string &email) {
    if (email.empty()) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Email must not be empty"
        );
    }
    for (char symbol : email) {
        if (std::isspace(symbol)) {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT,
                "Email must not contain spaces"
            );
        }
    }
    if (email.find('@') == std::string::npos) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Email must contain an @ symbol"
        );
    }
    if (email.find('.') == std::string::npos) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Email must contain a dot"
        );
    }
    if (email.find('@') > email.find('.')) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Email must contain a dot after the @ symbol"
        );
    }
    if (email.find('@') == 0) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Email must contain a username before the @ symbol"
        );
    }
    if (email.find('.') == email.size() - 1) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Email must contain a domain after the dot"
        );
    }
    if (email.find('@') == email.size() - 1) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Email must contain a domain after the @ symbol"
        );
    }
    return grpc::Status::OK;
}