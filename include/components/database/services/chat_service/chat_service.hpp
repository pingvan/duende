#pragma once

#include <vector>
#include <string>

#include "components/database/placeholders/placeholders.hpp"

namespace database {
struct chat_service_database_api {
    [[nodiscard]] static std::pair<uint32_t, std::string> add_message(const int &sender_id, const int &chat_id, const std::string &message_text);
    static void add_unhandle_message(const int &user_id, const int &message_id);

    [[nodiscard]] static std::vector<int> get_members_ids(const int &chat_id);

    [[nodiscard]] static user_chats get_user_chats(const int &user_id); //get status in chat
    [[nodiscard]] static last_messages get_chats_messages(const int &chat_id);
    [[nodiscard]] static chat_events get_chat_events(const int &chat_id);
    [[nodiscard]] static chat_preview get_chat_preview(const int &chat_id);


    [[nodiscard]] static invite::group_chat_preview get_group_chat_preview(const int &chat_id); //here we get preview to invite 
    // [[nodiscard]] static chat_preview get_private_chat_preview(const int &chat_id);

    [[nodiscard]] static bool check_user_in_chat(const int &user_id, const int &chat_id);

    [[nodiscard]] static std::pair<uint32_t, std::string> create_private_chat(const std::pair<int, int> &users_ids);

    [[nodiscard]] static std::pair<uint32_t, std::string> create_group_chat(const int &creator_id, const std::string &chat_name, const std::string &description, const std::vector<uint32_t> &users_in_chat);
    [[nodiscard]] static bool check_user_privileges(const int &user_id, const int &chat_id);

    static void add_user_to_chat(const int &chat_id, const int &user_id);
    static void remove_user_from_chat(const int &chat_id, const int &user_id);
    static void remove_user_from_chat(const int &chat_id, const int &admin_id, const int &user_id_who_remove);
    static void change_chat_name(const int &chat_id, const int &admin_id, const std::string &name);
    static void change_chat_description(const int &chat_id, const int &admin_id, const std::string &description);
    static void change_chat_cover(const int &chat_id, const int &admin_id, const std::basic_string<std::byte> &photo_binary);
    static void add_privileges_to_user(const int &chat_id, const int &user_id);
    static void remove_privileges_from_user(const int &chat_id, const int &user_id);

private:
    static chat_events_types string_to_enum(const std::string &event_type);

};
}