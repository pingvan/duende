#pragma once

#include <iostream>
#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <vector>

namespace database {

// Q::how to split database worker on services - form, user, authservice etc

// Q::replace std::string with std::string_view
//(optimization mb, but thinking longer we take string by reference, there is no, difference?)

struct connector {
    // authservice begin

    [[nodiscard]] static bool is_email_used(const std::string &user_email);
    [[nodiscard]] static bool is_nickname_used(const std::string &user_nickname);

    [[nodiscard]] static int get_user_id(const std::string &login);
    [[nodiscard]] static std::string get_user_email(const int &client_id);
    [[nodiscard]] static std::string get_user_nickname(const int &client_id);


    [[nodiscard]] static int add_user(
        const std::string &user_email,
        const std::string &user_nickname,
        const std::string &refresh_token,
        const std::string &password_hash,
        const std::string &password_salt
    );

    [[nodiscard]] static std::pair<std::string, std::string> get_hash_salt(
        const int &user_id
    );

    [[nodiscard]] static std::string get_refresh_token(const int &user_id);

    static void
    update_refresh_token(const int &user_id, const std::string &refresh_token);
    // authservice end


    // profiile service begin
    static int get_form_id(const int &client_id);

    static void change_email(const int &user_id, const std::string &new_user_email);
    static void change_nickname(const int &user_id, const std::string &new_user_nickname);

    static void
    change_photo(const int &client_id, const std::vector<unsigned char> &bytes);
    static void change_quote(const int &client_id, const std::string &quote);
    static void change_about(const int &client_id, const std::string &about);

    static void
    add_favourite_genre(const int &client_id, const std::string &genre);
    static void
    remove_favourite_genre(const int &client_id, const std::string &genre);

    static void add_favourite_actor(const int &client_id, const int &actor_id);
    static void
    remove_favourite_actor(const int &client_id, const int &actor_id);

    static void add_watched(
        const int &client_id,
        const int &film_id,
        const int &user_ratio
    );

    static void remove_watched(const int &client_id, const int &film_id);
    // profiile service end

    //front service start
    //front service end

    //chat service start
    //chat service end


private:
    [[nodiscard]] static std::
        pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>>
        open_connection();
};

}  // namespace database