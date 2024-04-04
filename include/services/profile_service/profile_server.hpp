#pragma once

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
using profile::Actor;
using profile::Genre;
using profile::Movie;
using profile::ProfileReply;
using profile::UsernameRequest;
using profile::MovieRequest;
using profile::ActorRequest;
using profile::GenreRequest;
using profile::BioRequest;
using profile::QuoteRequest;
using profile::ProfileService;

class ProfileServiceImpl final : public ProfileService::Service
{

  Status ChangeUsername(ServerContext *context, const UsernameRequest *request,
                         ProfileReply *reply) override;

  // Status ChangePhoto(ServerContext *context, const ProfileRequest *request,
  //                    ProfileReply *reply) override;

  Status ChangeQuote(ServerContext *context, const QuoteRequest *request,
                     ProfileReply *reply) override;

  Status ChangeBio(ServerContext *context, const BioRequest *request,
                    ProfileReply *reply) override;

  Status AddToWatchlist(ServerContext *context, const MovieRequest *request,
                       ProfileReply *reply) override;

  Status RemoveFromWatchlist(ServerContext *context, const MovieRequest *request,
                             ProfileReply *reply) override;

  Status AddToListOfActors(ServerContext *context, const ActorRequest *request,
                           ProfileReply *reply) override;

  Status RemoveFromListOfActors(ServerContext *context, const ActorRequest *request,
                                ProfileReply *reply) override;

  Status AddToListOfGenres(ServerContext *context, const GenreRequest *request,
                           ProfileReply *reply) override;

  Status RemoveFromListOfGenres(ServerContext *context, const GenreRequest *request,
                                ProfileReply *reply) override;
};
