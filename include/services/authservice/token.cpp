#include "token.hpp"
#include "components/jwt/jwt.hpp"
#include "components/jwt/secret_key.hpp"
#include "data.hpp"

namespace token_service {
authservice::Tokens *generate_tokens(authservice::UserDTO *user_dto) {
    authservice::Payload payload;
    payload.set_user_id(user_dto->id());
    payload.set_email(user_dto->email());
    payload.set_username(user_dto->username());
    authservice::AccessToken *access_token =
        jwtokens::create_access_token(payload, 30min);
    authservice::RefreshToken *refresh_token =
        jwtokens::create_refresh_token(payload, 43200min);
    authservice::Tokens *tokens = new authservice::Tokens();
    tokens->set_allocated_access(access_token);
    tokens->set_allocated_refresh(refresh_token);
    return tokens;
}

grpc::Status save_token(int user_id, authservice::RefreshToken token) {
    if (refresh_tokens.find(user_id) != refresh_tokens.end()) {
        refresh_tokens[user_id] = token;
    } else {
        refresh_tokens.insert({user_id, token});
    }
    return grpc::Status::OK;
}

authservice::UserDTO *get_user_dto(authservice::User user) {
    authservice::UserDTO *user_dto = new authservice::UserDTO();
    user_dto->set_email(user.email());
    user_dto->set_username(user.username());
    user_dto->set_id(user.id());
    return user_dto;
}

grpc::Status delete_refresh_token(const authservice::RefreshToken token) {
    for (const auto &[key, val] : refresh_tokens) {
        if (val.token() == token.token()) {
            refresh_tokens.erase(key);
            return grpc::Status::OK;
        }
    }
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Token not found");
}

grpc::Status find_refresh_token(const authservice::RefreshToken token) {
    for (const auto &[key, val] : refresh_tokens) {
        if (val.token() == token.token()) {
            return grpc::Status::OK;
        }
    }
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Token not found");
}

authservice::Payload parse_payload(std::string payload) {
    if (payload.empty()) {
        throw std::runtime_error("Payload is empty");
    }
    std::stringstream ss(payload);
    std::vector<std::string> parsed_payload;
    while (ss >> payload) {
        parsed_payload.push_back(payload);
    }
    if (parsed_payload.size() < 3) {
        throw std::runtime_error("Payload is invalid");
    }

    authservice::Payload user;
    try {
        user.set_user_id(std::stoi(parsed_payload[0]));
    } catch (...) {
        throw std::runtime_error("User id is invalid");
    }
    user.set_email(parsed_payload[1]);
    user.set_username(parsed_payload[2]);
    return user;
}

authservice::Payload validate_refresh_token(authservice::RefreshToken token) {
    try {
        authservice::Payload payload = jwtokens::decode_refresh_token(token);
        return payload;
    } catch (...) {
        authservice::Payload payload;
        payload.set_user_id(-1);
        return payload;
    }
}

authservice::Payload validate_access_token(authservice::AccessToken token) {
    try {
        authservice::Payload payload = jwtokens::decode_access_token(token);
        return payload;
    } catch (...) {
        authservice::Payload payload;
        payload.set_user_id(-1);
        return payload;
    }
}

authservice::Tokens *refresh(authservice::RefreshToken token) {
    authservice::Payload user_data = validate_refresh_token(token);
    grpc::Status find_token = find_refresh_token(token);
    if (user_data.user_id() == -1 || !find_token.ok()) {
        throw std::runtime_error("Authorization error");
    }
    authservice::User user;
    int id = user_data.user_id();
    for (const auto &[key, val] : users) {
        if (val.at("id") == std::to_string(id)) {
            user.set_email(val.at("email"));
            user.set_id(std::stoi(val.at("id")));
            user.set_username(key);
            user.set_salt(val.at("salt"));
            break;
        }
    }
    authservice::Payload payload;
    payload.set_user_id(user.id());
    payload.set_email(user.email());
    payload.set_username(user.username());
    authservice::AccessToken *access_token =
        jwtokens::create_access_token(payload, 30min);
    authservice::RefreshToken *refresh_token =
        jwtokens::create_refresh_token(payload, 43200min);
    authservice::Tokens *tokens = new authservice::Tokens();
    tokens->set_allocated_access(access_token);
    tokens->set_allocated_refresh(refresh_token);
    return tokens;
}
};  // namespace token_service
