#pragma once

#include <chrono>
#include "jwt-cpp/jwt.h"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

using namespace std::chrono;

namespace jwtokens {
authservice::AccessToken *
create_access_token(authservice::Payload info, minutes time);
authservice::RefreshToken *
create_refresh_token(authservice::Payload info, minutes time);
authservice::Payload decode_access_token(authservice::AccessToken);
authservice::Payload decode_refresh_token(authservice::RefreshToken);
}  // namespace jwtokens
