#include "front_service.hpp"

#include "components/database/connection/database-connector.hpp"

#include <iostream>

namespace database {

user_full_page front_service::get_user_form(const int &user_id) {
    try {
        const connector connnection;
        user_full_page form;
        std::string query = "SELECT * FROM actors "
                            "JOIN actors_to_form ON actors_to_form.actor_id = actors.id "
                            "WHERE actors_to_form.form_id = $1";
        std::string q = "SELECT * FROM films "
                        "JOIN views ON views.film_id = films.id "
                        "WHERE views.user_id = $1";
        const auto result_form = connnection.txn->exec_params("SELECT * FROM forms WHERE user_id = $1", user_id);
        int form_id = 0;
        for (const auto &row : result_form) {
            form_id = row[0].as<int>();
        }
        const auto result_actors = connnection.txn->exec_params(query, form_id);
        const auto result_films = connnection.txn->exec_params(q, user_id);
        std::cout << "all queries complete\n";
        return form;
    } catch(std::exception &e) {
        std::cout << e.what() << '\n';
        return user_full_page{};
    }
}



bool front_service::is_user_in_chat(const int &user_id, const int &chat_id) {
    const connector connnection;
    const auto result = connnection.txn->exec_params("SELECT EXISTS(SELECT 1 FROM chat_membership WHERE user_id = $1 AND chat_id = $2)", user_id, chat_id);
    //pqxx::zview z(ss); //ss is std::string_view
    bool is_user_in_chat = false;
    for (const auto &row : result) {
        is_user_in_chat = row[0].as<bool>();
    }
    connnection.txn->commit();
    return is_user_in_chat;
}

}


