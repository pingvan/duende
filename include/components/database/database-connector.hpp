#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace database {

//ask about split database worker on services - form, user, authservice (yes, it's too) and etc

//replace std::string with std::string_view (optimization????)

//repllace repeated open connection with define???

struct connector {
	//authservice begin
    static [[nodidcard]] bool is_email_used(const std::string &user_email);
    static [[nodidcard]] bool is_nickname_used(const std::string &user_nickname);

    static [[nodidcard]] int add_user(const std::string &user_email, const std::string &user_nickname,
        const std::string &auth_token, const std::string &refresh_token,
        const std::string &password_hash, const std::string &password_salt);

    static [[nodidcard]] std::pair<std::string, std::string> get_hash_salt(const int &user_id);

	static [[nodidcard]] std::string get_auth_token(const int &user_id);
    static [[nodidcard]] std::string get_refresh_token(const int &user_id);

	static void update_tokens(const int &user_id, const std::string &auth_token, const std::string &refresh_token);
	//authservice end


	//profiile service begin
    static void change_photo(const int &client_id, const std::vector<unsigned char> &bytes);
    static void change_quote(const int &client_id, const std::string &quote);
    static void change_about(const int &client_id, const std::string &about);

    static void add_favourite_genre();
    static void remove_favourite_genre();

	static void add_favourite_actor();
    static void remove_favourite_actor();

    static void add_watched(const int &film_id, const int &user_ratio);
	//profiile service end


	//also we need functions which returns values and fileds which would be shown in UI
	//it is form, photo...

private:
	static [[nodidcard]] std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>> open_connection();
	static void commit_and_close_connection(std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>>);

};

}