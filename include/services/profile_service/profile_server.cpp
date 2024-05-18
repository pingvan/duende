#include "profile_server.hpp"
#include "components/database/services/authservice/authservice.hpp"
#include "components/database/services/profile_service/profile_service.hpp"
#include <iostream>

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
    for (const auto &movie : current_profile.movies()) {
        database::profile_service_database_api::remove_watched(
            current_profile.id(), movie.id()
        );
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
        std::cout << "Removing genre on server: " << genre.name().c_str(); 
        database::profile_service_database_api::remove_favourite_genre(
            current_profile.id(), genre.name()
        );
    }
    reply->set_message("List of genres changed");
    return Status::OK;
}

// Status ProfileServiceImpl::GetProfile(
//     ServerContext *context,
//     const GetProfileRequest *request,
//     GetProfileReply *reply
// ) {
//     int id = request->id();
//     std::string username =
//         database::profile_service_database_api::get_username(id);
//     std::string quote = database::profile_service_database_api::get_quote(id);
//     std::string bio = database::profile_service_database_api::get_bio(id);
//     std::vector<Movie> movies =
//         database::profile_service_database_api::get_movies(id);
//     std::vector<Actor> actors =
//         database::profile_service_database_api::get_actors(id);
//     std::vector<Genre> genres =
//         database::profile_service_database_api::get_genres(id);

//     reply->set_username(username);
//     reply->set_quote(quote);
//     reply->set_bio(bio);

//     for (const auto &movie : movies) {
//         Movie *reply_movie = reply->add_movies();
//         reply_movie->set_id(movie.id());
//         reply_movie->set_rating(movie.rating());
//     }

//     for (const auto &actor : actors) {
//         Actor *reply_actor = reply->add_actors();
//         reply_actor->set_id(actor.id());
//     }

//     for (const auto &genre : genres) {
//         Genre *reply_genre = reply->add_genres();
//         reply_genre->set_name(genre.name());
//     }

//     return Status::OK;
// }
