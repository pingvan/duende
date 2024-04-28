#pragma once

#include <string>

#include "components/database/placeholders/placeholders.hpp"

//is everything solves with middlevare ???
//about:checking possibility to accsess data, 

//think about links to int's (rewirte or warns)

namespace database {
    struct invite_service_database_api {
        [[nodiscard]] static int add_invite(const std::string &invite_text, const invite_status &status, const int &invite_from, const int &invite_to);
        static void change_invite_status(const int &invite_id, const invite_status &new_status);
        static void add_unhandled_invite(const int &invite_to, const int &invite_id);

        [[nodiscard]] static bool is_invite_already_sended(const int &invite_from, const int &invite_to);

        [[nodiscard]] static invites_recivie get_received_invites(const int &user_id);
        [[nodiscard]] static invites_sent get_sended_invites(const int &user_id);

        [[nodiscard]] static std::string enum_invite_status_to_string(const invite_status &status);
        [[nodiscard]] static invite_status string_to_invite_status(const std::string &status);
    };
} //namespace database