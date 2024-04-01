#pragma once

#include "components/database/placeholders/palceholders.hpp"

namespace database {
struct front_service {
    // static user_preview get_user_preview();
    static user_full_page get_user_form(const int &user_id);
    static user_chats get_user_chats(const int &user_id);
    static last_messages get_chat_last_messages(const int &chat_id);
    static invites_recivie get_invite_received(const int &user_id);
    static invites_sent get_invites_sent(const int &user_id);

    static bool is_user_in_chat(const int &user_id, const int &chat_id);
};
}