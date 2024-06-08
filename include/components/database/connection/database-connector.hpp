#pragma once

#include <cassert>
#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <iostream>


namespace database {

struct authservice_database_api;
struct profile_service_database_api;
struct chat_service_database_api;
struct front_service;
struct invite_service_database_api;
struct search_service_database_api;
struct subscription_service_database_api;

struct databaseTesting;

struct connector {
    connector() {
        con = std::make_unique<pqxx::connection>(connection_string);
        txn = std::make_unique<pqxx::work>(*con);
    }

    
    connector(const std::string &connection_string_) {
        connection_string = connection_string_;
        con = std::make_unique<pqxx::connection>(connection_string);
        txn = std::make_unique<pqxx::work>(*con);
    }

    friend struct authservice_database_api;
    friend struct profile_service_database_api;
    friend struct chat_service_database_api;
    friend struct front_service;
    friend struct invite_service_database_api;
    friend struct search_service_database_api;
    friend struct subscription_service_database_api;
    friend struct databaseTesting;
    
    std::unique_ptr<pqxx::connection> con;
    std::unique_ptr<pqxx::work> txn;
    static inline std::string connection_string {"postgres://root:strongpassword@localhost:5432/duende"};
};

}  // namespace database