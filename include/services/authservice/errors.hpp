#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <map>
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"

using namespace authservice;

std::map<ServerError_ErrorType, std::string> error_messages = {
    {ServerError_ErrorType::ServerError_ErrorType_USER_ALREADY_EXISTS,
     "User already exists"},
    {ServerError_ErrorType::ServerError_ErrorType_USER_DOES_NOT_EXIST,
     "User does not exist"},
    {ServerError_ErrorType::ServerError_ErrorType_INVALID_USERNAME,
     "Invalid username"},
    {ServerError_ErrorType::ServerError_ErrorType_INVALID_PASSWORD,
     "Invalid password"},
    {ServerError_ErrorType::ServerError_ErrorType_INVALID_EMAIL,
     "Invalid email"},
    {ServerError_ErrorType::ServerError_ErrorType_INCORRECT_PASSWORD,
     "Incorrect password"},
    {ServerError_ErrorType::ServerError_ErrorType_UNEXPECTED_ERROR,
     "Unexpected error"},
    {ServerError_ErrorType::ServerError_ErrorType_PASSWORD_NOT_CONFIRMED, "Password not confirmed"},
};

#endif