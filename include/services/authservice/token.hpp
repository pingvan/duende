#pragma once

#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

#include <sstream>

namespace token_service {
    authservice::Tokens *generate_tokens(authservice::UserDTO *user_dto);
    grpc::Status save_token(int user_id, authservice::RefreshToken token);
    authservice::UserDTO *get_user_dto(authservice::User user);
    grpc::Status delete_refresh_token(const authservice::RefreshToken token);
    grpc::Status find_refresh_token(const authservice::RefreshToken token);
    authservice::Payload parse_payload(std::string payload);
    authservice::Payload validate_refresh_token(authservice::RefreshToken token);
    authservice::Payload validate_access_token(authservice::AccessToken token);
    authservice::Tokens *refresh(authservice::RefreshToken token);
}