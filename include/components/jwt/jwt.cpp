#include "jwt.hpp"

std::string jwt_tokens::get_env_var(const std::string &key) {
    const char *val = std::getenv(key.c_str());
    if (val == nullptr) {
        throw std::runtime_error("Environment variable " + key + " not set");
    }
    return std::string(val);
}

authservice::AccessToken *jwt_tokens::create_access_token(
    authservice::Payload info,
    std::chrono::seconds time
) {
    std::string secret_access = get_env_var("SECRET_ACCESS");

    authservice::AccessToken *token = new authservice::AccessToken();
    token->set_token(
        jwt::create()
            .set_issuer("auth0")
            .set_type("JWT")
            .set_issued_now()
            .set_expires_in(time)
            .set_payload_claim("id", jwt::claim(std::to_string(info.user_id())))
            .set_payload_claim("email", jwt::claim(info.email()))
            .set_payload_claim("username", jwt::claim(info.username()))
            .sign(jwt::algorithm::hs256{secret_access})
    );

    return token;
}

authservice::RefreshToken *jwt_tokens::create_refresh_token(
    authservice::Payload info,
    std::chrono::seconds time
) {
    std::string secret_refresh = get_env_var("SECRET_REFRESH");

    authservice::RefreshToken *token = new authservice::RefreshToken();
    token->set_token(
        jwt::create()
            .set_issuer("auth0")
            .set_type("JWT")
            .set_issued_now()
            .set_expires_in(time)
            .set_payload_claim("id", jwt::claim(std::to_string(info.user_id())))
            .set_payload_claim("email", jwt::claim(info.email()))
            .set_payload_claim("username", jwt::claim(info.username()))
            .sign(jwt::algorithm::hs256{secret_refresh})
    );

    return token;
}

authservice::Payload jwt_tokens::decode_token(const std::string &token) {
    auto decoded = jwt::decode(token);
    authservice::Payload user;
    for (auto payloads : decoded.get_payload_json()) {
        if (payloads.first == "id") {
            user.set_user_id(std::stoi(payloads.second.get<std::string>()));
        } else if (payloads.first == "email") {
            user.set_email(payloads.second.get<std::string>());
        } else if (payloads.first == "username") {
            user.set_username(payloads.second.get<std::string>());
        }
    }
    return user;
}
