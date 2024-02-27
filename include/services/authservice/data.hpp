#ifndef DATA_HPP_
#define DATA_HPP_

#include <unordered_map>
#include <vector>
#include "components/jwt/jwt.hpp"

inline std::unordered_map<std::string, std::pair<std::string, std::string>>
    users;

std::string jwt_token(std::string username);

#endif
