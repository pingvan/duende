#include "profile_service.hpp"
#include <components/database/connection/database-connector.hpp>

namespace database {
int profile_service::get_form_id(const int &user_id) {
    const connector connection;
    const auto get_form_id = connection.txn->exec_params("SELECT id FROM forms WHERE user_id = $1", user_id);
    int form_id = 0;
    for (auto row : get_form_id) {
        form_id = row[0].as<int>();
    }
    connection.txn->commit();
    return form_id;
}

void profile_service::change_email(const int &user_id, const std::string &new_user_email) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE users SET email = $1 WHERE id = $2", new_user_email, user_id);
    connection.txn->commit();
}

void profile_service::change_nickname(const int &user_id, const std::string &new_user_nickname) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE users SET login = $1 WHERE id = $2", new_user_nickname, user_id);
    connection.txn->commit();
}

void profile_service::change_photo(const int &user_id,
                             const std::vector<unsigned char> &bytes) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET photo = $1 WHERE user_id = $2", pqxx::binarystring(bytes.data(), bytes.size()), user_id);
    connection.txn->commit();
}

void profile_service::change_quote(const int &user_id, const std::string &quote) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET quote = $1 WHERE user_id = $2", quote, user_id);
    connection.txn->commit();
}

void profile_service::change_about(const int &user_id, const std::string &about) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET about = $1 WHERE user_id = $2", about, user_id);
    connection.txn->commit();
}

void profile_service::add_favourite_genre(const int &user_id, const std::string &genre) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET favourite_genres = ARRAY_APPEND(favourite_genres, $1) WHERE user_id = $2", genre, user_id);
    connection.txn->commit();
}

void profile_service::remove_favourite_genre(const int &user_id,
                                       const std::string &genre) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE forms SET favourite_genres = ARRAY_REMOVE(favourite_genres, $1) WHERE user_id = $2", genre, user_id);
    connection.txn->commit();
}


//or do the get before opnening second profile_servicection
void profile_service::add_favourite_actor(const int &user_id, const int &actor_id) {
    const connector connection;
    int form_id = get_form_id(user_id);
    const auto result = connection.txn->exec_params("INSERT INTO actors_to_form VALUES ($1, $2)", form_id, actor_id);
    connection.txn->commit();
}


void profile_service::remove_favourite_actor(const int &user_id,
                                       const int &actor_id) {
  const connector connection;
    int form_id = get_form_id(user_id);
    const auto result = connection.txn->exec_params("DELETE FROM actors_to_form WHERE form_id = $1 AND actor_id = $2", form_id, actor_id);
    connection.txn->commit();
}

void profile_service::add_watched(const int &user_id, const int &film_id,
                            const int &user_ratio) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO views (ratio, user_id, film_id) VALUES ($1, $2, $3)", user_ratio, user_id, film_id);
    connection.txn->commit();
}


void profile_service::remove_watched(const int &user_id, const int &film_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("DELETE FROM views WHERE user_id = $1 AND film_id = $2", user_id, film_id);
    connection.txn->commit();
}
}
