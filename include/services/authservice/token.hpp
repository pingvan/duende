#pragma once

#include <optional>
#include <sstream>
#include "components/jwt/jwt.hpp"
#include "components/jwt/secret_key.hpp"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

struct invalid_refresh_token : std::runtime_error {
    invalid_refresh_token()
        : std::runtime_error("Refresh token is invalid!\n") {
    }
};

struct token_service {
    authservice::Tokens *generate_tokens(authservice::UserDTO *user_dto);
    grpc::Status
    save_refresh_token(int user_id, authservice::RefreshToken token);
    authservice::UserDTO *get_user_dto(authservice::User user);
    grpc::Status find_refresh_token(const authservice::RefreshToken token);
    grpc::Status delete_refresh_token(const authservice::RefreshToken token);

    template <typename Token>
    grpc::Status validate_token(Token token) {
        try {
            jwt_tokens jwt_tokens;
            authservice::Payload payload = jwt_tokens.decode_token(token);
            return grpc::Status::OK;
        } catch (...) {
            return grpc::Status(
                grpc::StatusCode::INVALID_ARGUMENT, "Invalid token"
            );
        }
    }

    authservice::Tokens *refresh(authservice::RefreshToken token);
    grpc::Status activate_access_token(
        authservice::AccessToken token,
        authservice::Payload payload
    );
    grpc::Status activate_refresh_token(authservice::RefreshToken token);
};
