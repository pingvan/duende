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
      profiles[current_profile.id()].add_movies()->CopyFrom(movie);
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
  }

  Status RemoveFromWatchlist(ServerContext *context, const ProfileRequest *request,
                             ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    for (const auto &movie : current_profile.movies())
    {
      auto *watchlist = profiles[current_profile.id()].mutable_movies();
      auto current_movie = std::find_if(watchlist->begin(), watchlist->end(),
                                        [&movie](const Movie &m)
                                        {
                                          return m.id() == movie.id();
                                        });
      if (current_movie != watchlist->end())
      {
        watchlist->erase(current_movie);
      }
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
  }

  Status AddToListOfActors(ServerContext *context, const ProfileRequest *request,
                           ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    for (const auto &actor : current_profile.actors())
    {
      profiles[current_profile.id()].add_actors()->CopyFrom(actor);
    }
    reply->set_message("List of actors changed");
    return Status::OK;
  }

  Status RemoveFromListOfActors(ServerContext *context, const ProfileRequest *request,
                                ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    for (const auto &actor : current_profile.actors())
    {
      auto *list_of_actors = profiles[current_profile.id()].mutable_actors();
      auto current_actor = std::find_if(list_of_actors->begin(), list_of_actors->end(),
                                        [&actor](const Actor &a)
                                        {
                                          return a.id() == actor.id();
                                        });
      if (current_actor != list_of_actors->end())
      {
        list_of_actors->erase(current_actor);
      }
    }
    reply->set_message("List of actors changed");
    return Status::OK;
  }

  Status AddToListOfGenres(ServerContext *context, const ProfileRequest *request,
                           ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    for (const auto &genre : current_profile.genres())
    {
      profiles[current_profile.id()].add_genres()->CopyFrom(genre);
    }
    reply->set_message("List of genres changed");
    return Status::OK;
  }

  Status RemoveFromListOfGenres(ServerContext *context, const ProfileRequest *request,
                                ProfileReply *reply) override
  {
    ProfileRequest current_profile = *request;
    for (const auto &genre : current_profile.genres())
    {
      auto *list_of_genres = profiles[current_profile.id()].mutable_genres();
      auto current_genre = std::find_if(list_of_genres->begin(), list_of_genres->end(),
                                        [&genre](const Genre &g)
                                        {
                                          return g.id() == genre.id();
                                        });
      if (current_genre != list_of_genres->end())
      {
        list_of_genres->erase(current_genre);
      }
    }
    reply->set_message("List of genres changed");
    return Status::OK;
  }
};
