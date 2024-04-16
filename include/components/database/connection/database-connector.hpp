#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>


namespace database {

struct authservice_database_api;
struct profile_service_database_api;
struct chat_service_database_api;
struct front_service;
struct invite_service_database_api;
struct search_service_database_api;

struct connector {
    connector() {
        static std::string connection_string {"postgres://root:strongpassword@localhost:5432/duende"};
        con = std::make_unique<pqxx::connection>(connection_string);
        txn = std::make_unique<pqxx::work>(*con);
    }
private:

    friend struct authservice_database_api;
    friend struct profile_service_database_api;
    friend struct chat_service_database_api;
    friend struct front_service;
    friend struct invite_service_database_api;
    friend struct search_service_database_api;

    std::unique_ptr<pqxx::connection> con;
    std::unique_ptr<pqxx::work> txn;
};

}  // namespace database