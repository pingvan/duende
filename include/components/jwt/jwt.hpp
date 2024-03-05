#pragma once

#include <chrono>
#include "jwt-cpp/jwt.h"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

struct jwt_tokens {
    authservice::AccessToken *
    create_access_token(authservice::Payload info, std::chrono::seconds time);
    authservice::RefreshToken *
    create_refresh_token(authservice::Payload info, std::chrono::seconds time);
    authservice::Payload decode_token(const std::string &token);
};
