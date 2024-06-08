#pragma once

#include <grpc/grpc.h>
#include <services/search_service/search_service.grpc.pb.h>
#include <services/search_service/search_service.pb.h>

struct SearchService final : public search_service::SerachService::Service {
    grpc::Status SearchUserByCredentials(
        grpc::ServerContext *context,
        const search_service::UserSearchRequest *request,
        search_service::UserSearchResponse *response
    ) override;
    grpc::Status SearchFilmByTitle(
        grpc::ServerContext *context,
        const search_service::FilmSearchRequest *request,
        search_service::FilmSearchResponse *response
    ) override;
    grpc::Status SearchActorByName(
        grpc::ServerContext *context,
        const search_service::ActorSearchRequest *request,
        search_service::ActorSearchResponse *response
    ) override;
};