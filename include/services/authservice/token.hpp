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
    grpc::Status save_tokens(int user_id, authservice::RefreshToken refresh, authservice::AccessToken access);
    grpc::Status find_refresh_token(const authservice::RefreshToken token);
    grpc::Status validate_token(authservice::RefreshToken token);
    authservice::Tokens *refresh(authservice::RefreshToken token);
    grpc::Status activate_access_token(
        authservice::AccessToken token,
        authservice::Payload payload
    );
    grpc::Status is_refresh_token_active(authservice::RefreshToken token);
};
