#ifndef JWT_HPP_
#define JWT_HPP_

#include <chrono>
#include "jwt-cpp/jwt.h"

using namespace std::chrono;

namespace jwt {
    std::string create_jwt_token(std::string username);
}

#endif
