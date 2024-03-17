#pragma once

#include <pqxx/pqxx>
#include <string>

namespace database {
struct authservice {
    [[nodiscard]] static bool is_email_used(const std::string &user_email);
    [[nodiscard]] static bool is_nickname_used(const std::string &user_nickname);

    [[nodiscard]] static int get_user_id(const std::string &login);
    [[nodiscard]] static std::string get_user_email(const int &user_id);
    [[nodiscard]] static std::string get_user_nickname(const int &user_id);


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

    static void remove_refresh_token(const int &user_id);
};
}