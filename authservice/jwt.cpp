#include "jwt.hpp"

std::string jwt::create_jwt_token(std::string username) {
    auto current_time = duration_cast<seconds>(system_clock::now().time_since_epoch());
    auto expiration_time = current_time + seconds{864000}; // ten days

    std::string header = "JWThs256";
    std::string payload = std::to_string(expiration_time.count()) + " " + username;
    std::string signature = "secret";

    return jwt::create()
        .set_payload_claim("sub", jwt::claim(header))
        .set_payload_claim("exp", jwt::claim(payload))
        .sign(jwt::algorithm::hs256{signature});
}