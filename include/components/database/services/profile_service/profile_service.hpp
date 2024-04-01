#pragma once

#include <vector>
#include <string>

namespace database {
struct profile_service {
    static int get_form_id(const int &user_id);

    static void change_email(const int &user_id, const std::string &new_user_email);
    static void change_nickname(const int &user_id, const std::string &new_user_nickname);

    static void
    change_photo(const int &user_id, const std::vector<unsigned char> &bytes);
    static void change_quote(const int &user_id, const std::string &quote);
    static void change_about(const int &user_id, const std::string &about);

    static void
    add_favourite_genre(const int &user_id, const std::string &genre);
    static void
    remove_favourite_genre(const int &user_id, const std::string &genre);

    static void add_favourite_actor(const int &user_id, const int &actor_id);
    static void
    remove_favourite_actor(const int &user_id, const int &actor_id);

    static void add_watched(
        const int &user_id,
        const int &film_id,
        const int &user_ratio
    );

    static void remove_watched(const int &user_id, const int &film_id);
};
}