#include "search_service.hpp"
#include <iostream>
#include "components/database/services/search_service/search_service.hpp"

grpc::Status SearchService::SearchUserByCredentials(grpc::ServerContext *context, const search_service::UserSearchRequest *request, search_service::UserSearchResponse *response) {
    auto result = database::search_service_database_api::search_user_by_credentials(request->user_credentials());
    for (const auto &user : result) {
        auto *user_proto = response->add_users();
        user.convert_to_proto(user_proto);
    }
    return grpc::Status::OK;
}

grpc::Status SearchService::SearchActorByName(grpc::ServerContext *context, const search_service::ActorSearchRequest *request, search_service::ActorSearchResponse *response) {
    auto result = database::search_service_database_api::search_actor_by_name(request->actor_name());
    for (const auto &actor : result) {
        auto *actor_proto = response->add_actors();
        actor.convert_to_proto(actor_proto);
    }
    return grpc::Status::OK;
}

grpc::Status SearchService::SearchFilmByTitle(grpc::ServerContext *context, const search_service::FilmSearchRequest *request, search_service::FilmSearchResponse *response) {
        auto result = database::search_service_database_api::search_film_by_title(request->film_title());
        for (const auto &film : result) {
            auto *film_proto = response->add_films();
            film.convert_to_proto(film_proto);
        }
        return grpc::Status::OK;
    
}