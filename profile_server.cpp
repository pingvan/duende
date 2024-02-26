#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>


#include <grpcpp/grpcpp.h>

#include "proto/profile.grpc.pb.h"
#include "proto/profile.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using profile::ProfileReply;
using profile::ProfileRequest;
using profile::ProfileService;


class ProfileServiceImpl final : public ProfileService::Service {
  std::unordered_map<std::string, ProfileRequest> profiles;

  Status ChangeNickname(ServerContext *context, const ProfileRequest *request,
                        ProfileReply *reply){
    ProfileRequest current_profile = *request;
    bool nickname_exists = false;
    for (auto &p : profiles) {
      if (p.second.nickname() == current_profile.nickname()) {
        nickname_exists = true;
        break;
      }
    }
    if (nickname_exists) {
      reply->set_message("Nickname already exists");
      return Status::OK;
    }
    profiles[current_profile.id()].set_nickname(current_profile.nickname());
    reply->set_message("Nickname changed");
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  ProfileServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}