#include <string>
#include "data.hpp"

std::string jwt_token(std::string username) {
    return jwt::create_jwt_token(username);
}
