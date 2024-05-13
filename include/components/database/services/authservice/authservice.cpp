#include "authservice.hpp"
// #include <sys/types.h>
#include <cassert>
#include <components/database/connection/database-connector.hpp>
#include <exception>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace database {
bool authservice_database_api::is_email_used(const std::string &user_email) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT EXISTS(SELECT 1 FROM users WHERE email = $1)", user_email);
    connection.txn->commit();
    bool exists = false;
    for (auto row : result) {
        exists = row[0].as<bool>();
    }
    return exists;
}

bool authservice_database_api::is_nickname_used(const std::string &user_nickname) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT EXISTS(SELECT 1 FROM users WHERE login = $1)", user_nickname);
    connection.txn->commit();
    bool exists = false;
    for (auto row : result) {
        exists = row[0].as<bool>();
    }
    return exists;
}

int authservice_database_api::get_user_id(const std::string &login) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT id FROM users WHERE email = $1 OR login = $1", login);
    int id = -1;
    for (auto row : result) {
        id = row[0].as<int>();
    }
    connection.txn->commit();
    return id;
}


std::string authservice_database_api::get_user_email(const int &user_id) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT email FROM users WHERE id = $1", user_id);
    std::string email{};
    for (auto row : result) {
        email = row[0].as<std::string>();
    }
    connection.txn->commit();
    return email;
}

std::string authservice_database_api::get_user_nickname(const int &user_id) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT login FROM users WHERE id = $1", user_id);
    std::string nickname{};
    for (auto row : result) {
        nickname = row[0].as<std::string>();
    }
    connection.txn->commit();
    return nickname;
}


int authservice_database_api::add_user(const std::string &user_email, const std::string &user_nickname,
    const std::string &refresh_token,
    const std::string &password_hash, const std::string &password_salt) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO users (email, login) VALUES ($1, $2) RETURNING id", user_email, user_nickname);
    int user_id = -1;
    for (auto row : result) {
        user_id = row[0].as<int>();
    }
    connection.txn->exec_params("INSERT INTO tokens VALUES ($1, $2)", user_id, refresh_token);
    connection.txn->exec_params("INSERT INTO passwords VALUES ($1, $2, $3)", user_id, password_hash, password_salt);
    connection.txn->exec_params("INSERT INTO forms (user_id) VALUES ($1)", user_id);
    connection.txn->commit();
    return user_id;
}

std::pair<std::string, std::string> authservice_database_api::get_hash_salt(const int &user_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT password_hash, password_salt FROM passwords WHERE user_id = $1", user_id);
    std::pair<std::string, std::string> hash_salt_pair{};
    for (auto row : result) {
        hash_salt_pair.first = row[0].as<std::string>();
        hash_salt_pair.second = row[1].as<std::string>();
    }
    connection.txn->commit();
    return hash_salt_pair;
}

std::string authservice_database_api::get_refresh_token(const int &user_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT refresh_token FROM tokens WHERE user_id = $1", user_id);
    std::string refresh_token{};
    for (auto row : result) {
        refresh_token = row[0].as<std::string>();
    }
    connection.txn->commit();
    return refresh_token;
}

void authservice_database_api::update_refresh_token(const int &user_id,
                               const std::string &refresh_token) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE tokens SET refresh_token = $1 WHERE user_id = $2", refresh_token, user_id);
    connection.txn->commit();
}

void authservice_database_api::remove_refresh_token(const int &user_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("DELETE FROM tokens WHERE user_id = $1", user_id);
    connection.txn->commit();
}

user_preview authservice_database_api::get_user_preview(const int &user_id) {
    const connector connection;
    static constexpr std::string_view query = "SELECT u.login, f.photo "
                                              "FROM users u "
                                                "JOIN forms f ON u.id = f.user_id "
                                                "WHERE u.id = $1";
    const auto result = connection.txn->exec_params(pqxx::zview(query), user_id);
    user_preview prewiew;
    prewiew.id = user_id;
    for (const auto &row : result) {
        prewiew.login = row[0].as<std::string>();
        if (!row[1].is_null()) {
            prewiew.photo = row[1].as<std::basic_string<std::byte>>();
        }
    }
    connection.txn->commit();
    return prewiew;
}


//TO_ASK::very ambiguos, i think the other service would be more efficient and prettier...

user_full_page authservice_database_api::get_user_full_page(const int &user_id) {
    const connector connection;
    user_preview prewiew = get_user_preview(user_id);
    static std::string get_form = "SELECT id, quotation, about, favourite_genres FROM forms WHERE user_id = $1";
    const auto result = connection.txn->exec_params(get_form, user_id);
    user_full_page full_page;
    full_page.prewiew = prewiew;
    int form_id = -1;
    for (const auto &row : result) {
        form_id = row[0].as<int>();
        if (!row[1].is_null()) {
            full_page.quote = row[1].as<std::string>();
        }
        if (!row[2].is_null()) {
            full_page.about = row[2].as<std::string>();
        }
        if (!row[3].is_null()) {
            full_page.fauvorite_genres = array_parser(row[3].as<std::string>());
        }   
    }
    static std::string get_views = "SELECT ratio, title, description, genres "
                        "FROM VIEWS "
                        "INNER JOIN films ON views.film_id = films.id "
                        "WHERE user_id = $1";
    const auto result_second = connection.txn->exec_params(get_views, user_id);
    full_page.watched_films.reserve(result_second.size());
    for (const auto &row : result_second) {
        film_on_user_page film;
        film.ratio = row[0].as<int>();
        film.full_page.preview.title = row[1].as<std::string>();
        film.full_page.description = row[2].as<std::string>();
        film.full_page.genres = array_parser(row[3].as<std::string>());
        full_page.watched_films.push_back(film);
    }
    static std::string get_fauvorite_actors = "SELECT full_name "
                        "FROM actors_to_form "
                        "INNER JOIN actors ON actors_to_form.actor_id = actors.id "
                        "WHERE form_id = $1";
    const auto result_third = connection.txn->exec_params(get_fauvorite_actors, form_id);
    full_page.fauvorite_actors.reserve(result_third.size());
    for (const auto &row : result_third) {
        actor_preview actor_prewiew;
        actor_prewiew.full_name = row[0].as<std::string>();
        full_page.fauvorite_actors.push_back(actor_prewiew);
    }
    connection.txn->commit();
    return full_page;
}

genre authservice_database_api::enum_from_string(const std::string &ss) {
    static const std::unordered_map<std::string, genre> genre_map = {
        {"action", genre::ACTION},
        {"adventure", genre::ADVENTURE},
        {"comedy", genre::COMEDY},
        {"drama", genre::DRAMA},
        {"horror", genre::HORROR},
        {"sci-fi", genre::SCI_FI},
        {"science fiction", genre::SCI_FI},
        {"fantasy", genre::FANTASY},
        {"thriller", genre::THRILLER},
        {"mystery", genre::MYSTERY},
        {"romance", genre::ROMANCE},
        {"animation", genre::ANIMATION},
        {"documentary", genre::DOCUMENTARY},
        {"crime", genre::CRIME},
        {"western", genre::WESTERN},
        {"music", genre::MUSIC},
        {"history", genre::HISTORY},
        {"war", genre::WAR},
        {"biographical", genre::BIOGRAPHICAL},
        {"sports", genre::SPORTS},
        {"family", genre::FAMILY},
        {"foreign", genre::FOREIGN},
        {"tv movie", genre::TV_MOVIE}
    };

    if (auto it = genre_map.find(ss); it != genre_map.end()) {
        return it->second;
    } else {
        throw std::exception{};
    }
}

std::vector<genre> authservice_database_api::array_parser(const std::string &json_string) {
    constexpr uint32_t max_size = 13;
    constexpr uint32_t max_genres_amount = 22;
    static_assert(max_size == 13, "");
    static_assert(max_genres_amount == 22, "");
    
    std::vector<genre> genres;
    genres.reserve(max_genres_amount);
    //can optimize working with strings???
    std::string genre;
    genre.reserve(max_size);
    for (std::size_t i = 1; i < json_string.size(); i++) {
        if (json_string[i] == ',' || json_string[i] == '}') {
            genres.push_back(enum_from_string(genre));
            genre.clear();
            continue;
        }
        genre += json_string[i];
    }
    return genres;
}

}
