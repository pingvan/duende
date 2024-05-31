#pragma once

#include <string>

#include "components/database/placeholders/placeholders.hpp"

namespace database {
    struct invite_service_database_api {
        [[nodiscard]] static std::pair<uint32_t, std::string> add_invite(const std::string &invite_text, const int &invite_from, const int &invite_to);
        [[nodiscard]] static std::pair<uint32_t, std::string> add_invite(const std::string &invite_text, const int &invite_from, const int &invite_to, const int &chat_id);
        
        static void change_invite_status(const int &invite_id);
        static void add_unhandled_invite(const int &invite_to, const int &invite_id);

        [[nodiscard]] static bool is_invite_already_sent(const int &invite_from, const int &invite_to);
        [[nodiscard]] static bool is_invite_already_sent(const int &chat_id, const int &invite_to, bool group_chat = true);

        [[nodiscard]] static invites_recivie get_received_invites(const int &user_id);
        [[nodiscard]] static invites_sent get_sended_invites(const int &user_id);
    };
} //namespace database