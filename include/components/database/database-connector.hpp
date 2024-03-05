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
    [[nodiscard]] static bool is_email_used(const std::string &user_email);
    [[nodiscard]] static bool is_nickname_used(const std::string &user_nickname);

    //TODO::auth=accsess
    //return's user id

    //TODO::get_user_id

    [[nodiscard]] static int add_user(const std::string &user_email, const std::string &user_nickname,
        const std::string &auth_token, const std::string &refresh_token,
        const std::string &password_hash, const std::string &password_salt);

    [[nodiscard]] static std::pair<std::string, std::string> get_hash_salt(const int &user_id);

	[[nodiscard]] static std::string get_auth_token(const int &user_id);
    [[nodiscard]] static std::string get_refresh_token(const int &user_id);

	static void update_tokens(const int &user_id, const std::string &auth_token, const std::string &refresh_token);
	//authservice end


	//profiile service begin
	static int get_form_id(const int &client_id);

    static void change_photo(const int &client_id, const std::vector<unsigned char> &bytes);
    static void change_quote(const int &client_id, const std::string &quote);
    static void change_about(const int &client_id, const std::string &about);

    static void add_favourite_genre(const int &client_id, const std::string &genre);
    static void remove_favourite_genre(const int &client_id, const std::string &genre);

	static void add_favourite_actor(const int &client_id, const int &actor_id);
    static void remove_favourite_actor(const int &client_id, const int &actor_id);

    static void add_watched(const int &client_id, const int &film_id, const int &user_ratio);

    //TODO::remove watched
	//profiile service end


	//also we need functions which returns values and fileds which would be shown in UI
	//it is form, photo...

private:
	[[nodiscard]] static std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>> open_connection();
	static void commit_and_close_connection(std::pair<std::unique_ptr<pqxx::connection>, std::unique_ptr<pqxx::work>>);

};

}