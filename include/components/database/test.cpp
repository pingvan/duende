#include <pqxx/pqxx>

#include <iostream>
#include "database_connector.hpp"

int main() {
    std::string connect_string = "postgres://root:strongpassword@localhost:5432/duende";
    /*try {
        pqxx::connection con {connect_string};
        std::cout << "working\n";
        pqxx::work txn{con};
        auto rr = txn.exec_params("INSERT INTO clients (email, login) VALUES ($1, $2) RETURNING id", "ha@email.com", "nickname4");

        /*auto res = txn.exec("SELECT id FROM clients WHERE login =" + txn.quote("nickname"));#1#
        for (auto r : rr) {
            std::cout << r[0].as<int>() << '\n';
        }
        txn.commit();
        con.close();
    } catch (std::exception &connect_exception) {
        std::cerr << connect_exception.what() << '\n';
    }*/
    connector con;
    std::cout << con.is_email_used("ha@email.com");
    return 0;
}