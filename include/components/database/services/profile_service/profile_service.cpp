#include "profile_service.hpp"
#include <components/database/connection/database-connector.hpp>
#include <string_view>
#include "components/database/placeholders/placeholders.hpp"
#include "components/database/services/authservice/authservice.hpp"
#include "pqxx/zview.hxx"

namespace database {
int profile_service_database_api::get_form_id(const int &user_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT id FROM forms WHERE user_id = $1", user_id);
    int form_id = 0;
    for (const auto &row : result) {
        form_id = row[0].as<int>();
    }
    connection.txn->commit();
    return form_id;
}

void profile_service_database_api::change_email(const int &user_id, const std::string &new_user_email) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE users SET email = $1 WHERE id = $2", new_user_email, user_id);
    connection.txn->commit();
}

void profile_service_database_api::change_nickname(const int &user_id, const std::string &new_user_nickname) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE users SET login = $1 WHERE id = $2", new_user_nickname, user_id);
    connection.txn->commit();
}

void profile_service_database_api::change_photo(const int &user_id,
                             const std::basic_string<std::byte> &photo_binary) {
    const connector connection;
    // const auto result = connection.txn->exec_params("UPDATE forms SET photo = $1 WHERE user_id = $2", photo_binary, user_id);
    connection.txn->commit();
}

void profile_service_database_api::change_quotation(const int &user_id, const std::string &quotation) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET quotation = $1 WHERE user_id = $2", quotation, user_id);
    connection.txn->commit();
}

void profile_service_database_api::change_about(const int &user_id, const std::string &about) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET about = $1 WHERE user_id = $2", about, user_id);
    connection.txn->commit();
}

void profile_service_database_api::add_favourite_genre(const int &user_id, const std::string &genre) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET favourite_genres = ARRAY_APPEND(favourite_genres, $1) WHERE user_id = $2", genre, user_id);
    connection.txn->commit();
}

bool profile_service_database_api::check_is_favourite_genre(const int &user_id, const std::string &genre) {
    const connector connection;
    static constexpr std::string_view query = "SELECT $2 = ANY(favourite_genres) AS genre_exists FROM forms WHERE user_id = $1";
    const auto result = connection.txn->exec_params(pqxx::zview(query), user_id, genre);
    bool genre_exists = false;
    for (const auto &row : result) {
        genre_exists = row[0].as<bool>();
    }
    return genre_exists;
}

void profile_service_database_api::remove_favourite_genre(const int &user_id,
                                       const std::string &genre) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET favourite_genres = ARRAY_REMOVE(favourite_genres, $1) WHERE user_id = $2", genre, user_id);
    connection.txn->commit();
}


void profile_service_database_api::add_favourite_actor(const int &user_id, const int &actor_id) {
    const connector connection;
    int form_id = get_form_id(user_id);
    const auto result = connection.txn->exec_params("INSERT INTO actors_to_form VALUES ($1, $2)", form_id, actor_id);
    connection.txn->commit();
}

bool profile_service_database_api::check_is_favourite_actor(const int &user_id, const int &actor_id) {
    const connector connection;
    int form_id = get_form_id(user_id);
    static constexpr std::string_view query = "SELECT EXISTS(SELECT 1 FROM actors_to_form WHERE form_id = $1 AND actor_id = $2)";
    const auto result = connection.txn->exec_params(pqxx::zview(query), form_id, actor_id);
    bool actor_exists = false;
    for (const auto &row : result) {
        actor_exists = row[0].as<bool>();
    }
    return actor_exists;
}


void profile_service_database_api::remove_favourite_actor(const int &user_id,
                                       const int &actor_id) {
  const connector connection;
    const auto result = connection.txn->exec_params("DELETE FROM actors_to_form WHERE user_id = $1 AND actor_id = $2", user_id, actor_id);
    connection.txn->commit();
}

void profile_service_database_api::add_watched(const int &user_id, const int &film_id,
                            const int &user_ratio) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO views (ratio, user_id, film_id) VALUES ($1, $2, $3)", user_ratio, user_id, film_id);
    connection.txn->commit();
}

bool profile_service_database_api::check_is_watched(const int &user_id, const int &film_id) {
    const connector connection;
    static constexpr std::string_view query = "SELECT EXISTS(SELECT 1 FROM views WHERE user_id = $1 AND film_id = $2)";
    const auto result = connection.txn->exec_params(pqxx::zview(query), user_id, film_id);
    bool film_exists = false;
    for (const auto &row : result) {
        film_exists = row[0].as<bool>();
    }
    return film_exists;
}


void profile_service_database_api::remove_watched(const int &user_id, const int &film_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("DELETE FROM views WHERE user_id = $1 AND film_id = $2", user_id, film_id);
    connection.txn->commit();
}

film_preview profile_service_database_api::get_film_preview(const int &film_id) {
    const connector connection;
    static constexpr std::string_view query = "SELECT id, title, cover_preview_url FROM films WHERE id = $1";
    const auto result = connection.txn->exec_params(pqxx::zview(query), film_id);
    film_preview film;
    for (const auto &row : result) {
        film.id = row[0].as<int>();
        film.title = row[1].as<std::string>();
        if (!row[2].is_null()) {
            film.preview_cover_url = row[2].as<std::string>();
        } 
    }
    return film;
}

film_full_page profile_service_database_api::get_film_full_page(const int &film_id) {
    film_full_page film;
    film.preview = get_film_preview(film_id);
    const connector connection;
    static constexpr std::string_view query = "SELECT description, genres, cover_full_size_url FROM films WHERE id = $1";
    const auto result = connection.txn->exec_params(pqxx::zview(query), film_id);
    for (const auto &row : result) {
        film.description = row[0].as<std::string>();
        film.genres = authservice_database_api::array_parser(row[1].as<std::string>());
        if (!row[2].is_null()) {
            film.full_page_cover_url = row[2].as<std::string>();
        }
    }
    return film;
}
}
