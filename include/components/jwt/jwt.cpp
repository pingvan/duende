#include "jwt.hpp"
#include "secret_key.hpp"

authservice::AccessToken *jwtokens::create_access_token(authservice::Payload info, minutes time) {

    std::string payload = std::to_string(info.user_id()) + " " + info.email() + " " + info.username();

    authservice::AccessToken *token = new authservice::AccessToken();
    token->set_token(
        jwt::create()
            .set_type("JWS")
            .set_issuer("auth0")
            .set_payload_claim("payload", jwt::claim(payload))
            .sign(jwt::algorithm::hs256{secret_access_key})
    );

    return token;
}

authservice::RefreshToken *jwtokens::create_refresh_token(authservice::Payload info, minutes time) {

    std::string payload = std::to_string(info.user_id()) + " " + info.email() + " " + info.username();

    authservice::RefreshToken *token = new authservice::RefreshToken();
    token->set_token(
        jwt::create()
            .set_payload_claim("payload", jwt::claim(payload))
            .sign(jwt::algorithm::hs256{secret_refresh_key})
    );

    return token;
}

authservice::Payload jwtokens::decode_access_token(authservice::AccessToken token) {
    auto decoded = jwt::decode(token.token());
    std::string payload;
    for (auto payloads : decoded.get_payload_json()) {
        if (payloads.second.is<std::string>()) {
            payload += payloads.second.get<std::string>();
        } else if (payloads.second.is<double>()) {
            payload += std::to_string(payloads.second.get<double>());
        } else if (payloads.second.is<bool>()) {
            payload += payloads.second.get<bool>() ? "true" : "false";
        }
    }
    std::vector<std::string> parsed_payload;
    std::stringstream ss(payload);
    std::string item;
    while (std::getline(ss, item, ' ')) {
        parsed_payload.push_back(item);
    }
    if (parsed_payload.size() < 3) {
        throw std::runtime_error("Payload is invalid");
    }

    authservice::Payload user;
    try {
        user.set_user_id(std::stoi(parsed_payload[0]));
    }
    catch (...) {
        throw std::runtime_error("User id is invalid");
    }
    user.set_email(parsed_payload[1]);
    user.set_username(parsed_payload[2]);
    return user;
}

authservice::Payload jwtokens::decode_refresh_token(authservice::RefreshToken token) {
    auto decoded = jwt::decode(token.token());
    std::string payload;
    for (auto payloads : decoded.get_payload_json()) {
        if (payloads.second.is<std::string>()) {
            payload += payloads.second.get<std::string>();
        } else if (payloads.second.is<double>()) {
            payload += std::to_string(payloads.second.get<double>());
        } else if (payloads.second.is<bool>()) {
            payload += payloads.second.get<bool>() ? "true" : "false";
        }
    }
    std::vector<std::string> parsed_payload;
    std::stringstream ss(payload);
    std::string item;
    while (std::getline(ss, item, ' ')) {
        parsed_payload.push_back(item);
    }
    if (parsed_payload.size() < 3) {
        throw std::runtime_error("Payload is invalid");
    }

    authservice::Payload user;
    try {
        user.set_user_id(std::stoi(parsed_payload[0]));
    }
    catch (...) {
        throw std::runtime_error("User id is invalid");
    }
    user.set_email(parsed_payload[1]);
    user.set_username(parsed_payload[2]);
    return user;
}