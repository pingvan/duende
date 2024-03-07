#include "profile_server.hpp"
#include "components/database/database-connector.hpp"

Status ProfileServiceImpl::ChangeNickname(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    if (db_connector.is_nickname_used(current_profile.nickname())) {
        reply->set_message("Nickname already exists");
        return Status::OK;
    }
    db_connector.change_nickname(current_profile.id(), current_profile.nickname());
    reply->set_message("Nickname changed");
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
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    if (current_profile.quote().length() > 100) {
        reply->set_message("Quote is too long");
        return Status::OK;
    }
    database::connector db_connector;
    db_connector.change_quote(current_profile.id(), current_profile.quote());
    reply->set_message("Quote changed");
    return Status::OK;
}

Status ProfileServiceImpl::ChangeBio(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    if (current_profile.bio().length() > 1000) {
        reply->set_message("Bio is too long");
        return Status::OK;
    }
    db_connector.change_about(current_profile.id(), current_profile.bio());
    reply->set_message("Bio changed");
    return Status::OK;
}

Status ProfileServiceImpl::AddToWatchlist(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    for (const auto &movie : current_profile.movies()) {
        db_connector.add_watched(current_profile.id(), movie.id(), movie.rating());
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
}

Status ProfileServiceImpl::RemoveFromWatchlist(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    for (const auto &movie : current_profile.movies()) {
        db_connector.remove_watched(current_profile.id(), movie.id());
    }
    reply->set_message("Watchlist changed");
    return Status::OK;
}

// Status ProfileServiceImpl::UpdateMovieRating(
//     ServerContext *context,
//     const ProfileRequest *request,
//     ProfileReply *reply
// ) {
//     ProfileRequest current_request = *request;
//     database::connector db_connector;
//     std::string sql = "UPDATE watchlist SET rating = " 
//                     + std::to_string(current_request.new_rating()) 
//                     + " WHERE profile_id = " + current_request.profile_id() 
//                     + " AND movie_id = " + current_request.movie_id();
//     db_connector.execute_query(sql);
//     reply->set_message("Rating updated");
//     return Status::OK;
// }

Status ProfileServiceImpl::AddToListOfActors(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    for (const auto &actor : current_profile.actors()) {
        db_connector.add_favourite_actor(current_profile.id(), actor.id());
    }
    reply->set_message("List of actors changed");
    return Status::OK;
}

Status ProfileServiceImpl::RemoveFromListOfActors(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    for (const auto &actor : current_profile.actors()) {
        db_connector.remove_favourite_actor(current_profile.id(), actor.id());
    }
    reply->set_message("List of actors changed");
    return Status::OK;
}

Status ProfileServiceImpl::AddToListOfGenres(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    for (const auto &genre : current_profile.genres()) {
        db_connector.add_favourite_genre(current_profile.id(), genre.name());
    }
    reply->set_message("List of genres changed");
    return Status::OK;
}

Status ProfileServiceImpl::RemoveFromListOfGenres(
    ServerContext *context,
    const ProfileRequest *request,
    ProfileReply *reply
) {
    ProfileRequest current_profile = *request;
    database::connector db_connector;
    for (const auto &genre : current_profile.genres()) {
        db_connector.remove_favourite_genre(current_profile.id(), genre.name());
    }
    reply->set_message("List of genres changed");
    return Status::OK;
}
