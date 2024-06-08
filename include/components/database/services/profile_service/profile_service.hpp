#pragma once

#include <string>
#include <vector>
#include "components/database/placeholders/placeholders.hpp"

namespace database {
struct profile_service_database_api {
    [[nodiscard]] static int get_form_id(const int &user_id);

    static void
    change_email(const int &user_id, const std::string &new_user_email);
    static void
    change_nickname(const int &user_id, const std::string &new_user_nickname);

    static void change_photo(
        const int &user_id,
        const std::basic_string<std::byte> &photo_binary
    );
    static void
    change_quotation(const int &user_id, const std::string &quotation);
    static void change_about(const int &user_id, const std::string &about);

    static void
    add_favourite_genre(const int &user_id, const std::string &genre);
    [[nodiscard]] static bool
    check_is_favourite_genre(const int &user_id, const std::string &genre);
    static void
    remove_favourite_genre(const int &user_id, const std::string &genre);

    static void add_favourite_actor(const int &user_id, const int &actor_id);
    [[nodiscard]] static bool
    check_is_favourite_actor(const int &user_id, const int &actor_id);
    static void remove_favourite_actor(const int &user_id, const int &actor_id);

    static void
    add_watched(const int &user_id, const int &film_id, const int &user_ratio);
    [[nodiscard]] static bool
    check_is_watched(const int &user_id, const int &film_id);
    static void remove_watched(const int &user_id, const int &film_id);
    [[nodiscard]] static film_preview get_film_preview(const int &film_id);
    [[nodiscard]] static film_full_page get_film_full_page(const int &film_id);
};
}  // namespace database