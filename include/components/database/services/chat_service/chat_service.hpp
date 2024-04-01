#pragma once

#include <vector>
#include <string>

namespace database {
struct chat_service {
    [[nodiscard]] static int add_message(const int &sender_id, const int &chat_id, const std::string &time, const std::string &message_text);
    static void add_unhandle_message(const int &user_id, const int &message_id);

    [[nodiscard]] static std::vector<int> get_members_ids(const int &chat_id);
};
}