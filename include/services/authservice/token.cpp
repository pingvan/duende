#include "token.hpp"
#include "components/jwt/jwt.hpp"
#include "data.hpp"

authservice::Tokens *token_service::generate_tokens(
    authservice::UserDTO *user_dto
) {
    authservice::Payload payload;
    payload.set_user_id(user_dto->id());
    payload.set_email(user_dto->email());
    payload.set_username(user_dto->username());

    jwt_tokens jwt_tokens;
    authservice::AccessToken *access_token =
        jwt_tokens.create_access_token(payload, std::chrono::seconds{30 * 60});
    authservice::RefreshToken *refresh_token = jwt_tokens.create_refresh_token(
        payload, std::chrono::seconds{30 * 24 * 60 * 60}
    );
    authservice::Tokens *tokens = new authservice::Tokens();
    tokens->set_allocated_access(access_token);
    tokens->set_allocated_refresh(refresh_token);

    return tokens;
}

grpc::Status token_service::save_refresh_token(
    int user_id,
    authservice::RefreshToken token
) {
    if (refresh_tokens.find(user_id) != refresh_tokens.end()) {
        refresh_tokens[user_id] = token;
    } else {
        refresh_tokens.insert({user_id, token});
    }
    return grpc::Status::OK;
}

authservice::UserDTO *token_service::get_user_dto(authservice::User user) {
    authservice::UserDTO *user_dto = new authservice::UserDTO();
    user_dto->set_email(user.email());
    user_dto->set_username(user.username());
    user_dto->set_id(user.id());
    return user_dto;
}

grpc::Status token_service::find_refresh_token(
    const authservice::RefreshToken token
) {
    for (const auto &[key, val] : refresh_tokens) {
        if (val.token() == token.token()) {
            return grpc::Status::OK;
        }
    }
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Token not found");
}

grpc::Status token_service::delete_refresh_token(
    const authservice::RefreshToken token
) {
    for (const auto &[key, val] : refresh_tokens) {
        if (val.token() == token.token()) {
            refresh_tokens.erase(key);
            return grpc::Status::OK;
        }
    }
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Token not found");
}

authservice::Tokens *token_service::refresh(authservice::RefreshToken token) {
    if (!validate_token(token.token()).ok() ||
        !find_refresh_token(token).ok()) {
        throw invalid_refresh_token();
    }

    authservice::User user;
    jwt_tokens jwt_tokens;
    int id = jwt_tokens.decode_token(token.token()).user_id();
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
        jwt_tokens.create_access_token(payload, std::chrono::seconds{30 * 60});
    authservice::RefreshToken *refresh_token = jwt_tokens.create_refresh_token(
        payload, std::chrono::seconds{30 * 24 * 60 * 60}
    );
    authservice::Tokens *tokens = new authservice::Tokens();
    tokens->set_allocated_access(access_token);
    tokens->set_allocated_refresh(refresh_token);
    return tokens;
}

grpc::Status token_service::activate_access_token(
    authservice::AccessToken token,
    authservice::Payload payload
) {
    try {
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_ACCESS})
            .with_issuer("auth0")
            .verify(jwt::decode(token.token()));
    } catch (...) {
        if (activate_refresh_token(refresh_tokens[payload.user_id()]).ok()) {
            jwt_tokens jwt_tokens;
            jwt_tokens.create_access_token(
                payload, std::chrono::seconds{30 * 60}
            );
        } else {
            refresh(refresh_tokens[payload.user_id()]);
        }
    }
    return grpc::Status::OK;
}

grpc::Status token_service::activate_refresh_token(
    authservice::RefreshToken token
) {
    try {
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_REFRESH})
            .with_issuer("auth0")
            .verify(jwt::decode(token.token()));
        return grpc::Status::OK;
    } catch (...) {
        return grpc::Status(
            grpc::StatusCode::UNAUTHENTICATED, "Token is invalid"
        );
    }
}