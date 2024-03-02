#include "server.hpp"

grpc::Status auth_service::password_is_valid(const std::string &password) {
    int digit_count = 0;
    int upper_count = 0;
    int lower_count = 0;
    for (char symbol : password) {
        if (isdigit(symbol)) {
            digit_count++;
        }
        if (isupper(symbol)) {
            upper_count++;
        }
        if (islower(symbol)) {
            lower_count++;
        }
        if (std::isspace(symbol)) {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT,
                "Password must not contain spaces"
            );
        }
    }
    if (password.size() < 8) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Password is too short. It must be at least 8 characters long"
        );
    }
    if (digit_count == 0) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Password must contain a digit"
        );
    }
    if (upper_count == 0) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Password must contain an uppercase letter"
        );
    }
    if (lower_count == 0) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Password must contain a lowercase letter"
        );
    }
    return grpc::Status::OK;
}

grpc::Status auth_service::username_is_valid(const std::string &username) {
    int upper_count = 0;
    int lower_count = 0;
    for (char symbol : username) {
        if (isupper(symbol)) {
            upper_count++;
        }
        if (islower(symbol)) {
            lower_count++;
        }
        if (std::isspace(symbol)) {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT,
                "Username must not contain spaces"
            );
        }
    }
    if (username.size() < 4) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Username is too short. It must be at least 4 characters long"
        );
    }
    if (username.size() > 20) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Username is too long. It must be at most 20 characters long"
        );
    }
    if (upper_count > 0) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Username must not contain uppercase letters"
        );
    }
    if (lower_count == 0) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "Username must contain a lowercase letter"
        );
    }
    return grpc::Status::OK;
}

grpc::Status auth_service::email_is_valid(const std::string &email) {
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