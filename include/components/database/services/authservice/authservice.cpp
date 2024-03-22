#include "authservice.hpp"
#include <components/database/connection/database-connector.hpp>

namespace database {
bool authservice::is_email_used(const std::string &user_email) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT EXISTS(SELECT 1 FROM users WHERE email = $1)", user_email);
    connection.txn->commit();
    bool exists = false;
    for (auto row : result) {
        exists = row[0].as<bool>();
    }
    return exists;

}

bool authservice::is_nickname_used(const std::string &user_nickname) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT EXISTS(SELECT 1 FROM users WHERE login = $1)", user_nickname);
    connection.txn->commit();
    bool exists = false;
    for (auto row : result) {
        exists = row[0].as<bool>();
    }
    return exists;
}

int authservice::get_user_id(const std::string &login) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT id FROM users WHERE email = $1 OR login = $1", login);
    int id = -1;
    for (auto row : result) {
        id = row[0].as<int>();
    }
    connection.txn->commit();
    return id;
}


std::string authservice::get_user_email(const int &user_id) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT email FROM users WHERE id = $1", user_id);
    std::string email{};
    for (auto row : result) {
        email = row[0].as<std::string>();
    }
    connection.txn->commit();
    return email;
}

std::string authservice::get_user_nickname(const int &user_id) {
    const connector connection;
    auto result = connection.txn->exec_params("SELECT login FROM users WHERE id = $1", user_id);
    std::string nickname{};
    for (auto row : result) {
        nickname = row[0].as<std::string>();
    }
    connection.txn->commit();
    return nickname;
}



int authservice::add_user(const std::string &user_email, const std::string &user_nickname,
    const std::string &refresh_token,
    const std::string &password_hash, const std::string &password_salt) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO users (email, login) VALUES ($1, $2) RETURNING id", user_email, user_nickname);
    int user_id = 0;
    for (auto row : result) {
        user_id = row[0].as<int>();
    }
    connection.txn->exec_params("INSERT INTO tokens VALUES ($1, $2)", user_id, refresh_token);
    connection.txn->exec_params("INSERT INTO passwords VALUES ($1, $2, $3)", user_id, password_hash, password_salt);
    connection.txn->exec_params("INSERT INTO forms (user_id) VALUES ($1)", user_id);
    connection.txn->commit();
    return user_id;
}

std::pair<std::string, std::string> authservice::get_hash_salt(const int &user_id) {
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

std::string authservice::get_refresh_token(const int &user_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT refresh_token FROM tokens WHERE user_id = $1", user_id);
    std::string refresh_token{};
    for (auto row : result) {
        refresh_token = row[0].as<std::string>();
    }
    connection.txn->commit();
    return refresh_token;
}

void authservice::update_refresh_token(const int &user_id,
                               const std::string &refresh_token) {
    const connector connection;
    const auto result = connection.txn->exec_params("UPDATE tokens SET refresh_token = $1 WHERE user_id = $2", refresh_token, user_id);
    connection.txn->commit();
}

void authservice::remove_refresh_token(const int &user_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("DELETE FROM tokens WHERE user_id = $1", user_id);
}
}
