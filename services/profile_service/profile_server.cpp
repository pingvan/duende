#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <grpcpp/grpcpp.h>

#include "services/profile_service/profile.grpc.pb.h"
#include "services/profile_service/profile.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using profile::ProfileReply;
using profile::ProfileRequest;
using profile::ProfileService;

class ProfileServiceImpl final : public ProfileService::Service
{
  std::unordered_map<std::string, ProfileRequest> profiles;

  Status ChangeNickname(ServerContext *context, const ProfileRequest *request,
                        ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    bool nickname_exists = false;
    for (auto &p : profiles)
    {
      if (p.second.nickname() == current_profile.nickname())
      {
        nickname_exists = true;
        break;
      }
    }
    if (nickname_exists)
    {
      reply->set_message("Nickname already exists");
      return Status::OK;
    }
    profiles[current_profile.id()].set_nickname(current_profile.nickname());
    reply->set_message("Nickname changed");
    return Status::OK;
  }

  Status ChangePhoto(ServerContext *context, const ProfileRequest *request,
                     ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    profiles[current_profile.id()].set_photo(current_profile.photo());
    reply->set_message("Photo changed");
    return Status::OK;
  }

  Status ChangeQuote(ServerContext *context, const ProfileRequest *request,
                     ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    if (current_profile.quote().length() > 100)
    {
      reply->set_message("Quote is too long");
      return Status::OK;
    }
    profiles[current_profile.id()].set_quote(current_profile.quote());
    reply->set_message("Quote changed");
    return Status::OK;
  }

  Status ChangeBio(ServerContext *context, const ProfileRequest *request,
                   ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    if (current_profile.bio().length() > 1000)
    {
      reply->set_message("Bio is too long");
      return Status::OK;
    }
    profiles[current_profile.id()].set_bio(current_profile.bio());
    reply->set_message("Bio changed");
    return Status::OK;
  }

  Status AddToWatchlist(ServerContext *context, const ProfileRequest *request,
                        ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    for (const auto &movie : current_profile.movies())
    {
      profiles[current_profile.id()].add_movie()->CopyFrom(movie);
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
  }

Status RemoveFromWatchlist(ServerContext *context, const ProfileRequest *request,
                           ProfileReply *reply) override {
  ProfileRequest current_profile = *request;
  for (const auto &movie : current_profile.movies()) {
    auto* watchlist = profiles[current_profile.id()].mutable_movie();
    auto current_movie = std::find_if(watchlist->begin(), watchlist->end(),
                           [&movie](const Movie& m) {
                             return m.id() == movie.id();
                           });
    if (current_movie != watchlist->end()) {
      watchlist->erase(current_movie);
    }
  }
  reply->set_message("Watchlist changed");
  return Status::OK;
}

  // TODO: ChangeWatchlist, ChangeActors, ChangeJenres
};

// void RunServer() {
//   std::string server_address("0.0.0.0:50051");
//   ProfileServiceImpl service;

//   ServerBuilder builder;
//   builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//   builder.RegisterService(&service);
//   std::unique_ptr<Server> server(builder.BuildAndStart());
//   server->Wait();
// }

// int main(int argc, char** argv) {
//   RunServer();
//   return 0;
// }