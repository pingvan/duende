#include <pqxx/pqxx>

#include <memory>
#include <cassert>

#include "database-connector.hpp"

//is it possible to avoid code copy-pasting?

#define openCon auto working_pair = open_connection(); \
                const auto &txn = working_pair.second;

//TODO::add creating form at user_add function

namespace database {


std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>> connector::open_connection() {
    static constexpr std::string connection_string = "postgres://root:strongpassword@localhost:5432/duende";
    auto connection_ptr = std::make_unique<pqxx::connection>(connection_string);
    auto transaction_ptr = std::make_unique<pqxx::work>(*connection_ptr);
    return std::make_pair(std::move(connection_ptr), std::move(transaction_ptr));
}

void connector::commit_and_close_connection(std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>> p) {
    p.second->commit();
    p.first->close();
}


bool connector::is_email_used(const std::string &user_email) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("SELECT EXISTS(SELECT 1 FROM clients WHERE email = $1)", user_email);
    commit_and_close_connection(std::move(working_pair));
    bool res = false;
    for (auto row : result) {
        res = row[0].as<bool>();
    }
    return res;

}

bool connector::is_nickname_used(const std::string &user_nickname) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    auto result = txn->exec_params("SELECT EXISTS(SELECT 1 FROM clients WHERE nickname = $1)", user_nickname);
    commit_and_close_connection(std::move(working_pair));
    bool res = false;
    for (auto row : result) {
        res = row[0].as<bool>();
    }
    return res;
}


int connector::add_user(const std::string &user_email, const std::string &user_nickname,
    const std::string &auth_token, const std::string &refresh_token,
    const std::string &password_hash, const std::string &password_salt) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("INSERT INTO clients (email, login) VALUES ($1, $2) RETURNING id", user_email, user_nickname);
    int id = 0;
    for (auto row : result) {
        id = row[0].as<int>();
    }
    txn->exec_params("INSERT INTO tokens VALUES ($1, $2, $3)", id, auth_token, refresh_token);
    txn->exec_params("INSERT INTO passwords VALUES ($1, $2, $3)", id, password_hash, password_salt);
    commit_and_close_connection(std::move(working_pair));
    return id;
}

std::pair<std::string, std::string> connector::get_hash_salt(const int &user_id) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
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
    commit_and_close_connection(std::move(working_pair));
    return hash_salt_pair;
}

std::string connector::get_auth_token(const int &user_id) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("SELECT auth_token FROM tokens WHERE client_id = $1", user_id);
    std::string auth_token{};
    for (auto row : result) {
        auth_token = row[0].as<std::string>();
    }
    commit_and_close_connection(std::move(working_pair));
    return auth_token;
}

std::string connector::get_refresh_token(const int &user_id) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("SELECT refresh_token FROM tokens WHERE client_id = $1", user_id);
    std::string refresh_token{};
    for (auto row : result) {
        refresh_token = row[0].as<std::string>();
    }
    commit_and_close_connection(std::move(working_pair));
    return refresh_token;
}

void connector::update_tokens(const int &user_id,
                               const std::string &auth_token,
                               const std::string &refresh_token) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("UPDATE tokens SET auth_token = $1, refresh_token = $2 WHERE client_id = $3", auth_token, refresh_token, user_id);
    commit_and_close_connection(std::move(working_pair));
}

//what name is better to use client_id or user_id (imho client better according to application structure)
void connector::change_photo(const int &client_id,
                             const std::vector<unsigned char> &bytes) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("UPDATE form SET photo = $1 WHERE client_id = $2", bytes, client_id);
    commit_and_close_connection(std::move(working_pair));
}

void connector::change_quote(const int &client_id, const std::string &quote) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("UPDATE form SET quote = $1 WHERE client_id = $2", quote, client_id);
    commit_and_close_connection(std::move(working_pair));
}

void connector::change_about(const int &client_id, const std::string &about) {
    auto working_pair = open_connection();
    const auto &txn = working_pair.second;
    const auto result = txn->exec_params("UPDATE form SET about = $1 WHERE client_id = $2", about, client_id);
    commit_and_close_connection(std::move(working_pair));
}


}

