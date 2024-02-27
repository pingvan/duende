#include "data.hpp"
#include <string>

std::string jwt_token(std::string username) {
    return jwt::create_jwt_token(username);
}
