#include "token.hpp"
#include "components/jwt/jwt.hpp"
#include "components/database/database-connector.hpp"

authservice::Tokens *token_service::generate_tokens(
    authservice::Payload payload
) {

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

grpc::Status token_service::save_refresh_token(int user_id, authservice::RefreshToken token) {
    database::connector connector;
    connector.update_refresh_token(user_id, token.token());
    return grpc::Status::OK;
}

grpc::Status token_service::validate_token(authservice::RefreshToken token) {
    try {
        jwt_tokens jwt_tokens;
        authservice::Payload payload = jwt_tokens.decode_token(token.token());
        return grpc::Status::OK;
    } catch (...) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT, "Invalid token"
        );
    }
}

authservice::Tokens *token_service::refresh(authservice::RefreshToken token) {
    if (!validate_token(token).ok()) {
        throw invalid_refresh_token();
    }

    jwt_tokens jwt_tokens;
    int id = jwt_tokens.decode_token(token.token()).user_id();

    authservice::Payload payload;
    database::connector connector;
    payload.set_user_id(id);
    payload.set_email(connector.get_user_email(id));
    payload.set_username(connector.get_user_nickname(id));

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
    int user_id,
    std::string token,
    authservice::Payload payload
) {
    try {
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_ACCESS})
            .with_issuer("auth0")
            .verify(jwt::decode(token));
    } catch (...) {
        database::connector connector;
        if (is_refresh_token_active(connector.get_refresh_token(user_id)).ok()) {
            jwt_tokens jwt_tokens;
            jwt_tokens.create_access_token(
                payload, std::chrono::seconds{30 * 60}
            );
        } else {
            return grpc::Status(
                grpc::StatusCode::UNAUTHENTICATED, "Token is invalid"
            );
        }
    }
    return grpc::Status::OK;
}

grpc::Status token_service::is_refresh_token_active(
    std::string token
) {
    try {
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_REFRESH})
            .with_issuer("auth0")
            .verify(jwt::decode(token));
        return grpc::Status::OK;
    } catch (...) {
        return grpc::Status(
            grpc::StatusCode::UNAUTHENTICATED, "Token is invalid"
        );
    }
}