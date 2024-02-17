
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "profile.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using profile::Profile;
using profile::ProfileReply;
using profile::ProfileRequest;
using profile::ProfileService;

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

class GreeterServiceImpl final : public Greeter::Service {
  std::unordered_map<int, Profile> profiles;

  Status ChangeNickname(ServerContext *context, const ProfileRequest *request,
                        ProfileReply *reply) override {
    Profile profile = request->profile();
    bool nickname_exists = false;
    for (auto &p : profiles) {
      if (p.second.nickname() == profile.nickname()) {
        nickname_exists = true;
        break;
      }
    }
    if (nickname_exists) {
      reply->set_message("Nickname already exists");
      return Status::OK;
    }
    profiles[profile.id()].nickname() = profile.nickname();
    reply->set_message("Nickname changed");
    return Status::OK;
  }

  Status ChangePhoto(ServerContext *context, const ProfileRequest *request,
                     ProfileReply *reply) override {
    Profile profile = request->profile();
    profiles[profile.id()].set_photo(profile.photo());
    reply->set_message("Photo changed");
    return Status::OK;
  }

  Status DeletePhoto(ServerContext *context, const ProfileRequest *request,
                     ProfileReply *reply) override {
    Profile profile = request->profile();
    profiles[profile.id()].clear_photo();
    reply->set_message("Photo deleted");
    return Status::OK;
  }

  Status ChangeQuote(ServerContext *context, const ProfileRequest *request,
                     ProfileReply *reply) override {
    Profile profile = request->profile();
    if (profile.quote().length() > 100) {
      reply->set_message("Quote is too long");
      return Status::OK;
    }
    profiles[profile.id()].set_quote(profile.quote());
    reply->set_message("Quote changed");
    return Status::OK;
  }

  // мб не надо
  Status DeleteQuote(ServerContext *context, const ProfileRequest *request,
                     ProfileReply *reply) override {
    Profile profile = request->profile();
    profiles[profile.id()].clear_quote();
    reply->set_message("Quote deleted");
    return Status::OK;
  }

  Status ChangeBio(ServerContext *context, const ProfileRequest *request,
                   ProfileReply *reply) override {
    Profile profile = request->profile();
    if (profile.bio().length() > 1000) {
      reply->set_message("Bio is too long");
      return Status::OK;
    }
    profiles[profile.id()].set_bio(profile.bio());
    reply->set_message("Bio changed");
    return Status::OK;
  }

  // мб не надо
  Status DeleteBio(ServerContext *context, const ProfileRequest *request,
                   ProfileReply *reply) override {
    Profile profile = request->profile();
    profiles[profile.id()].clear_bio();
    reply->set_message("Bio deleted");
    return Status::OK;
  }

  // TODO: ChangeWatchlist, ChangeFavouriteActors, ChangeFavouriteJenres (выбор из списка, хз как пока что)
};


void RunServer(uint16_t port) {
  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);
  RunServer(absl::GetFlag(FLAGS_port));
  return 0;
}
