#pragma once

#include <vector>
#include <string>

#include "components/database/placeholders/placeholders.hpp"

namespace database {
struct chat_service_database_api {
    [[nodiscard]] static int add_message(const int &sender_id, const int &chat_id, const std::string &time, const std::string &message_text);
    static void add_unhandle_message(const int &user_id, const int &message_id);

    [[nodiscard]] static std::vector<int> get_members_ids(const int &chat_id);

    static user_chats get_user_chats(const int &user_id);
    static last_messages get_chats_messages(const int &chat_id);

    static bool check_user_in_chat(const int &user_id, const int &chat_id);

    static int create_private_chat(const std::pair<int, int> &users_ids, const std::string &timestamp);
};
}