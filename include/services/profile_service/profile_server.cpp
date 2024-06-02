#include "profile_server.hpp"
#include <iostream>
#include "components/database/services/authservice/authservice.hpp"
#include "components/database/services/profile_service/profile_service.hpp"

Status ProfileServiceImpl::ChangeUsername(
    ServerContext *context,
    const UsernameRequest *request,
    ProfileReply *reply
) {
    UsernameRequest current_profile = *request;
    if (database::authservice_database_api::is_nickname_used(
            current_profile.username()
        )) {
        return Status(grpc::ALREADY_EXISTS, "Username already exists");
    }
    database::profile_service_database_api::change_nickname(
        current_profile.id(), current_profile.username()
    );
    reply->set_message("Username changed");
    return Status::OK;
}

Status ProfileServiceImpl::ChangeQuote(
    ServerContext *context,
    const QuoteRequest *request,
    ProfileReply *reply
) {
    QuoteRequest current_profile = *request;
    if (current_profile.quote().length() > 100) {
        return Status(grpc::INVALID_ARGUMENT, "Quote is too long");
    }
    database::profile_service_database_api::change_quotation(
        current_profile.id(), current_profile.quote()
    );
    reply->set_message("Quote changed");
    return Status::OK;
}

Status ProfileServiceImpl::ChangeBio(
    ServerContext *context,
    const BioRequest *request,
    ProfileReply *reply
) {
    BioRequest current_profile = *request;
    if (current_profile.bio().length() > 1000) {
        return Status(grpc::INVALID_ARGUMENT, "Bio is too long");
    }
    database::profile_service_database_api::change_about(
        current_profile.id(), current_profile.bio()
    );
    reply->set_message("Bio changed");
    return Status::OK;
}

Status ProfileServiceImpl::AddToWatchlist(
    ServerContext *context,
    const MovieRequest *request,
    ProfileReply *reply
) {
    MovieRequest current_profile = *request;
    for (const auto &movie : current_profile.movies()) {
        if (database::profile_service_database_api::check_is_watched(
                current_profile.id(), movie.id()
            )) {
            return Status(grpc::ALREADY_EXISTS, "Movie already in watchlist");
        }
        database::profile_service_database_api::add_watched(
            current_profile.id(), movie.id(), movie.rating()
        );
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
}

Status ProfileServiceImpl::RemoveFromWatchlist(
    ServerContext *context,
    const MovieRequest *request,
    ProfileReply *reply
) {
    MovieRequest current_profile = *request;
    try {
        for (const auto &movie : current_profile.movies()) {
            database::profile_service_database_api::remove_watched(
                current_profile.id(), movie.id()
            );
        }
        reply->set_message("Watchlist changed");
        return Status::OK;
    } catch (const std::exception &e) {
        std::string error_message = "Failed to change watchlist: ";
        error_message += e.what();
        std::cerr << error_message
                  << std::endl;
        return Status(grpc::INTERNAL, error_message);
    }
}

Status ProfileServiceImpl::AddToListOfActors(
    ServerContext *context,
    const ActorRequest *request,
    ProfileReply *reply
) {
    ActorRequest current_profile = *request;
    for (const auto &actor : current_profile.actors()) {
        if (database::profile_service_database_api::check_is_favourite_actor(
                current_profile.id(), actor.id()
            )) {
            return Status(grpc::ALREADY_EXISTS, "Actor already in list");
        }
        database::profile_service_database_api::add_favourite_actor(
            current_profile.id(), actor.id()
        );
    }
    reply->set_message("List of actors changed");
    return Status::OK;
}

Status ProfileServiceImpl::RemoveFromListOfActors(
    ServerContext *context,
    const ActorRequest *request,
    ProfileReply *reply
) {
    ActorRequest current_profile = *request;
    for (const auto &actor : current_profile.actors()) {
        database::profile_service_database_api::remove_favourite_actor(
            current_profile.id(), actor.id()
        );
    }
    reply->set_message("List of actors changed");
    return Status::OK;
}

Status ProfileServiceImpl::AddToListOfGenres(
    ServerContext *context,
    const GenreRequest *request,
    ProfileReply *reply
) {
    GenreRequest current_profile = *request;
    for (const auto &genre : current_profile.genres()) {
        database::profile_service_database_api::add_favourite_genre(
            current_profile.id(), genre.name()
        );
    }
    reply->set_message("List of genres changed");
    return Status::OK;
}

Status ProfileServiceImpl::RemoveFromListOfGenres(
    ServerContext *context,
    const GenreRequest *request,
    ProfileReply *reply
) {
    GenreRequest current_profile = *request;
    for (const auto &genre : current_profile.genres()) {
        database::profile_service_database_api::remove_favourite_genre(
            current_profile.id(), genre.name()
        );
    }
    reply->set_message("List of genres changed");
    return Status::OK;
}

Status ProfileServiceImpl::ChangeProfilePhoto(
    ServerContext *context,
    const ProfilePhotoRequest *request,
    ProfileReply *reply
) {
    int user_id = request->user_id();

    std::string photo_data = request->photo();
    std::basic_string<std::byte> photo_binary(
        reinterpret_cast<const std::byte *>(photo_data.data()),
        photo_data.size()
    );

    database::profile_service_database_api::change_photo(user_id, photo_binary);

    reply->set_message("Profile photo changed");
    return Status::OK;
}

grpc::Status ProfileDataService::GetFilmFullPage(grpc::ServerContext *context, const profile::FilmId *request, common::FilmFullPage *response) {
    auto film_full_page = database::profile_service_database_api::get_film_full_page(request->id());
    film_full_page.convert_to_proto(response);
    return grpc::Status::OK;
}
