#pragma once

#include <vector>
#include <string>

#include "components/database/placeholders/placeholders.hpp"

namespace database {
struct chat_service_database_api {
    [[nodiscard]] static std::pair<uint, std::string> add_message(const int &sender_id, const int &chat_id, const std::string &message_text);
    static void add_unhandle_message(const int &user_id, const int &message_id);

    [[nodiscard]] static std::vector<int> get_members_ids(const int &chat_id);

    [[nodiscard]] static user_chats get_user_chats(const int &user_id);
    [[nodiscard]] static last_messages get_chats_messages(const int &chat_id);

    [[nodiscard]] static bool check_user_in_chat(const int &user_id, const int &chat_id);

    [[nodiscard]] static std::pair<uint, std::string> create_private_chat(const std::pair<int, int> &users_ids); 
};
}