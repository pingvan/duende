#include <pqxx/pqxx>

#include <memory>

#include "database_connector.hpp"

std::unique_ptr<pqxx::work> connector::open_connection() {
    pqxx::connection con{"postgres://root:strongpassword@localhost:5432/duende"};
    auto work_ptr = std::make_unique<pqxx::work>(con);
    return work_ptr;
}

void connector::close_connection(pqxx::work &txn) {
    txn.commit();
    txn.conn().close();
}

bool connector::is_email_used(const std::string &user_email) {
    pqxx::connection con{"postgres://root:strongpassword@localhost:5432/duende"};
    pqxx::work txn{con};
    auto result = txn.exec_params("SELECT EXISTS(SELECT 1 FROM clients WHERE email = $1)", user_email);
    txn.commit();
    con.close();
    bool res = false;
    for (auto row : result) {
        res = row[0].as<bool>();
    }
    return res;

}

bool connector::is_nickname_used(const std::string &user_nickname) {
    pqxx::connection con{"postgres://root:strongpassword@localhost:5432/duende"};
    pqxx::work txn{con};
    auto result = txn.exec_params("SELECT EXISTS(SELECT 1 FROM clients WHERE nickname = $1)", user_nickname);
    txn.commit();
    con.close();
    bool res = false;
    for (auto row : result) {
        res = row[0].as<bool>();
    }
    return res;
}



void connector::clear_all() {
    pqxx::connection con{"postgres://root:strongpassword@localhost:5432/duende"};
    pqxx::work txn{con};
    txn.exec("TRUNCATE clients, passwords, tokens, forms, producers, views, films, producers_by_film");
    txn.commit();
    con.close();
}

void connector::add_user(const std::string &user_email, const std::string &user_nickname,
    const std::string &auth_token, const std::string &refresh_token,
    const std::string &password_hash, const std::string &password_salt) {
    pqxx::connection con{"postgres://root:strongpassword@localhost:5432/duende"};
    pqxx::work txn{con};
    auto result = txn.exec_params("INSERT INTO clients (email, login) VALUES ($1, $2) RETURNING id", user_email, user_nickname);
    int id = 0;
    for (auto row : result) {
        id = row[0].as<int>();
    }
    txn.exec_params("INSERT INTO tokens VALUES ($1, $2, $3)", id, auth_token, refresh_token);
    txn.exec_params("INSERT INTO passwords VALUES ($1, $2, $3)", id, password_hash, password_salt);
    txn.commit();
    con.close();
}

