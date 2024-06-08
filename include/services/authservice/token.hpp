#pragma once

#include <optional>
#include <sstream>
#include "components/jwt/jwt.hpp"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

struct invalid_refresh_token : std::runtime_error {
    invalid_refresh_token()
        : std::runtime_error("Refresh token is invalid!\n") {
    }
};

struct token_service {
    authservice::Tokens *generate_tokens(authservice::Payload payload);
    grpc::Status
    save_refresh_token(int user_id, authservice::RefreshToken token);
    grpc::Status validate_token(authservice::RefreshToken token);
    authservice::RefreshToken *refresh_refresh_token(
        authservice::RefreshToken token
    );
    authservice::AccessToken *refresh_access_token(
        authservice::RefreshToken refresh_token
    );
    grpc::Status is_access_token_active(std::string token);
    grpc::Status is_refresh_token_active(std::string token);
};