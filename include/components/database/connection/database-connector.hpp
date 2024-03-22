#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>


namespace database {

struct authservice;
struct profile_service;
struct chat_service;
struct front_service;

struct connector {
    connector() {
        static std::string connection_string {"postgres://root:strongpassword@localhost:5432/duende"};
        con = std::make_unique<pqxx::connection>(connection_string);
        txn = std::make_unique<pqxx::work>(*con);
    }
private:

    friend struct authservice;
    friend struct profile_service;
    friend struct chat_service;
    friend struct front_service;

    std::unique_ptr<pqxx::connection> con;
    std::unique_ptr<pqxx::work> txn;
};

}  // namespace database