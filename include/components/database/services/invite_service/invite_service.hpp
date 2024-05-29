#pragma once

#include <string>

#include "components/database/placeholders/placeholders.hpp"

//is everything solves with middlevare ???
//about:checking possibility to accsess data, 

//think about links to int's (rewirte or warns)

namespace database {
    struct invite_service_database_api {
        [[nodiscard]] static std::pair<uint32_t, std::string> add_invite(const std::string &invite_text, const int &invite_from, const int &invite_to);
        static void change_invite_status(const int &invite_id);
        static void add_unhandled_invite(const int &invite_to, const int &invite_id);

        [[nodiscard]] static bool is_invite_already_sended(const int &invite_from, const int &invite_to);

        [[nodiscard]] static invites_recivie get_received_invites(const int &user_id);
        [[nodiscard]] static invites_sent get_sended_invites(const int &user_id);
    };
} //namespace database