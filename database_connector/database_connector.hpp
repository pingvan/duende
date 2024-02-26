#pragma once

#include <string>
#include <vector>
#include <iostream>

//use ILIKE to find films

struct connector {
    static std::unique_ptr<pqxx::work> open_connection();

    static void close_connection(pqxx::work &txn);

    static bool is_email_used(const std::string &user_email);
    static bool is_nickname_used(const std::string &user_nickname);

    static void add_user(const std::string &user_email, const std::string &user_nickname,
        const std::string &auth_token, const std::string &refresh_token,
        const std::string &password_hash, const std::string &password_salt);

    static std::pair<std::string, std::string> get_hash_salt();



    static void change_photo(const int &client_id, const std::vector<unsigned char> &bytes);
    static void change_quote(const int &client_id, const std::string &quote);
    static void change_about(const int &client_id, const std::string &about);

    static void add_favourite_genre();
    static void remove_favourite_genre();

    static void add_watched(const int &film_id, const int &user_ratio);



    static void replace_tokens(const int &user_id, const std::string &auth_token, const std::string &refresh_token);

    static std::string get_auth_token(const int &user_id);
    static std::string get_refresh_token(const int &user_id);



    static void clear_all();

private:
    // static constexpr std::string connect_string = "postgres://root:strongpassword@localhost:5432/duende";
};