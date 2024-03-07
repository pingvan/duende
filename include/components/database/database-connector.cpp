#include <pqxx/pqxx>

#include "database-connector.hpp"

//Q::naming convention - better client_... or user_... everywhere

//Q::is it possible to avoid code copy-pasting with defines?

#define openCon auto working_pair = open_connection(); \
                const auto &txn = working_pair.second;



namespace database {

//Q::what is the best way to make static constexpr??
std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>> connector::open_connection() {
    static std::string connection_string {"postgres://root:strongpassword@localhost:5432/duende"};
    auto connection_ptr = std::make_unique<pqxx::connection>(connection_string);
    auto transaction_ptr = std::make_unique<pqxx::work>(*connection_ptr);
    return std::make_pair(std::move(connection_ptr), std::move(transaction_ptr));
}


bool connector::is_email_used(const std::string &user_email) {
    openCon
    const auto result = txn->exec_params("SELECT EXISTS(SELECT 1 FROM clients WHERE email = $1)", user_email);
    txn->commit();
    bool exists = false;
    for (auto row : result) {
        exists = row[0].as<bool>();
    }
    return exists;

}

bool connector::is_nickname_used(const std::string &user_nickname) {
    openCon
    auto result = txn->exec_params("SELECT EXISTS(SELECT 1 FROM clients WHERE nickname = $1)", user_nickname);
    txn->commit();
    bool exists = false;
    for (auto row : result) {
        exists = row[0].as<bool>();
    }
    return exists;
}

int connector::get_user_id(const std::string &login) {
    openCon
    auto result = txn->exec_params("SELECT id FROM clients WHERE email = $1 OR login = $1", login);
    int id = -1;
    for (auto row : result) {
        id = row[0].as<int>();
    }
    txn->commit();
    return id;
}


std::string connector::get_user_email(const int &client_id) {
    openCon
    auto result = txn->exec_params("SELECT email FROM clients WHERE id = $1", client_id);
    std::string email{};
    for (auto row : result) {
        email = row[0].as<std::string>();
    }
    txn->commit();
    return email;
}

std::string connector::get_user_nickname(const int &client_id) {
    openCon
    auto result = txn->exec_params("SELECT login FROM clients WHERE id = $1", client_id);
    std::string nickname{};
    for (auto row : result) {
        nickname = row[0].as<std::string>();
    }
    txn->commit();
    return nickname;
}



int connector::add_user(const std::string &user_email, const std::string &user_nickname,
    const std::string &refresh_token,
    const std::string &password_hash, const std::string &password_salt) {
    openCon
    const auto result = txn->exec_params("INSERT INTO clients (email, login) VALUES ($1, $2) RETURNING id", user_email, user_nickname);
    int client_id = 0;
    for (auto row : result) {
        client_id = row[0].as<int>();
    }
    txn->exec_params("INSERT INTO tokens VALUES ($1, $2)", client_id, refresh_token);
    txn->exec_params("INSERT INTO passwords VALUES ($1, $2, $3)", client_id, password_hash, password_salt);
    txn->exec_params("INSERT INTO forms (client_id) VALUES ($1)", client_id);
    txn->commit();
    return client_id;
}

std::pair<std::string, std::string> connector::get_hash_salt(const int &user_id) {
    openCon
    const auto result = txn->exec_params("SELECT password_hash, password_salt FROM passwords WHERE client_id = $1", user_id);
    int counter = 0;
    std::pair<std::string, std::string> hash_salt_pair{};
    for (auto row : result) {
        if (counter == 0) {
            hash_salt_pair.first = row[0].as<std::string>();
            ++counter;
        } else {
            assert(counter == 1);
            hash_salt_pair.first = row[0].as<std::string>();
        }
    }
    txn->commit();
    return hash_salt_pair;
}

std::string connector::get_refresh_token(const int &user_id) {
    openCon
    const auto result = txn->exec_params("SELECT refresh_token FROM tokens WHERE client_id = $1", user_id);
    std::string refresh_token{};
    for (auto row : result) {
        refresh_token = row[0].as<std::string>();
    }
    txn->commit();
    return refresh_token;
}

void connector::update_refresh_token(const int &user_id,
                               const std::string &refresh_token) {
    openCon
    const auto result = txn->exec_params("UPDATE tokens SET refresh_token = $1 WHERE client_id = $2", refresh_token, user_id);
    txn->commit();
}

//what name is better to use client_id or user_id (imho client better according to application structure)


int connector::get_form_id(const int &client_id) {
    openCon
    const auto get_form_id = txn->exec_params("SELECT id FROM forms WHERE client_id = $1", client_id);
    int form_id = 0;
    for (auto row : get_form_id) {
        form_id = row[0].as<int>();
    }
    txn->commit();
    return form_id;
}

void connector::change_email(const int &user_id, const std::string &new_user_email) {
    openCon
    const auto result = txn->exec_params("UPDATE clients SET email = $1 WHERE id = $2", new_user_email, user_id);
    txn->commit();
}

void connector::change_nickname(const int &user_id, const std::string &new_user_nickname) {
    openCon
    const auto result = txn->exec_params("UPDATE clients SET login = $1 WHERE id = $2", new_user_nickname, user_id);
    txn->commit();
}

void connector::change_photo(const int &client_id,
                             const std::vector<unsigned char> &bytes) {
    openCon
    const auto result = txn->exec_params("UPDATE forms SET photo = $1 WHERE client_id = $2", pqxx::binarystring(bytes.data(), bytes.size()), client_id);
    txn->commit();
}

void connector::change_quote(const int &client_id, const std::string &quote) {
    openCon
    const auto result = txn->exec_params("UPDATE form SET quote = $1 WHERE client_id = $2", quote, client_id);
    txn->commit();
}

void connector::change_about(const int &client_id, const std::string &about) {
    openCon
    const auto result = txn->exec_params("UPDATE form SET about = $1 WHERE client_id = $2", about, client_id);
    txn->commit();
}

void connector::add_favourite_genre(const int &client_id, const std::string &genre) {
    openCon
    const auto result = txn->exec_params("UPDATE forms SET favourite_genres = ARRAY_APPEND(favourite_genres, $1) WHERE client_id = $2", genre, client_id);
    txn->commit();
}

void connector::remove_favourite_genre(const int &client_id,
                                       const std::string &genre) {
    openCon
    const auto result = txn->exec_params("UPDATE forms SET favourite_genres = ARRAY_REMOVE(favourite_genres, $1) WHERE client_id = $2", genre, client_id);
    txn->commit();
}


//or do the get before opnening second connection
void connector::add_favourite_actor(const int &client_id, const int &actor_id) {
    openCon
    int form_id = get_form_id(client_id);
    const auto result = txn->exec_params("INSERT INTO actors_to_form VALUES ($1, $2)", form_id, actor_id);
    txn->commit();
}


void connector::remove_favourite_actor(const int &client_id,
                                       const int &actor_id) {
  openCon
    int form_id = get_form_id(client_id);
    const auto result = txn->exec_params("DELETE FROM actors_to_form WHERE form_id = $1 AND actor_id = $2", form_id, actor_id);
    txn->commit();
}

void connector::add_watched(const int &client_id, const int &film_id,
                            const int &user_ratio) {
    openCon
    const auto result = txn->exec_params("INSERT INTO views (ratio, client_id, film_id) VALUES ($1, $2, $3)", user_ratio, client_id, film_id);
    txn->commit();
}


void connector::remove_watched(const int &client_id, const int &film_id) {
    openCon
    const auto result = txn->exec_params("DELETE FROM views WHERE client_id = $1 AND film_id = $2", client_id, film_id);
    txn->commit();
}

}