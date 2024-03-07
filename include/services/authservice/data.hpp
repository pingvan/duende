#pragma once

#include <unordered_map>
#include <vector>
#include "components/jwt/jwt.hpp"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

inline std::
    unordered_map<std::string, std::unordered_map<std::string, std::string>>
        users;

inline std::unordered_map<int, authservice::RefreshToken> refresh_tokens;
