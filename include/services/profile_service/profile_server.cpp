#include "profile_server.hpp"
#include <components/database/services/authservice/authservice.hpp>

#include "components/database/services/profile_service/profile_service.hpp"

Status ProfileServiceImpl::ChangeUsername(
    ServerContext *context,
    const UsernameRequest *request,
    ProfileReply *reply
) {
    UsernameRequest current_profile = *request;
    if (database::authservice::is_nickname_used(current_profile.username())) {
        reply->set_message("Username already exists");
        return Status::OK;
    }
    database::profile_service::change_nickname(current_profile.id(), current_profile.username());
    reply->set_message("Username changed");
    return Status::OK;
}

// Status ProfileServiceImpl::ChangePhoto(
//     ServerContext *context,
//     const ProfileRequest *request,
//     ProfileReply *reply
// ) {
//     ProfileRequest current_profile = *request;
//     profiles[current_profile.id()].set_photo(current_profile.photo());
//     reply->set_message("Photo changed");
//     return Status::OK;
// }

Status ProfileServiceImpl::ChangeQuote(
    ServerContext *context,
    const QuoteRequest *request,
    ProfileReply *reply
) {
    QuoteRequest current_profile = *request;
    if (current_profile.quote().length() > 100) {
        reply->set_message("Quote is too long");
        return Status::OK;
    }
    database::profile_service::change_quote(current_profile.id(), current_profile.quote());
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
        reply->set_message("Bio is too long");
        return Status::OK;
    }
    database::profile_service::change_about(current_profile.id(), current_profile.bio());
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
        database::profile_service::add_watched(current_profile.id(), movie.id(), movie.rating());
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
    for (const auto &movie : current_profile.movies()) {
        database::profile_service::remove_watched(current_profile.id(), movie.id());
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
}

Status ProfileServiceImpl::AddToListOfActors(
    ServerContext *context,
    const ActorRequest *request,
    ProfileReply *reply
) {
    ActorRequest current_profile = *request;
    for (const auto &actor : current_profile.actors()) {
         database::profile_service::add_favourite_actor(current_profile.id(), actor.id());
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
        database::profile_service::remove_favourite_actor(current_profile.id(), actor.id());
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
        database::profile_service::add_favourite_genre(current_profile.id(), genre.name());
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
        database::profile_service::remove_favourite_genre(current_profile.id(), genre.name());
    }
    reply->set_message("List of genres changed");
    return Status::OK;
}
