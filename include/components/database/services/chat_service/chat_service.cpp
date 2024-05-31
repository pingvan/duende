#include "chat_service.hpp"
#include <components/database/connection/database-connector.hpp>
#include <string>
#include <string_view>
#include <vector>
#include "components/database/placeholders/placeholders.hpp"
#include "components/database/services/authservice/authservice.hpp"
#include "pqxx/zview.hxx"

namespace database {
std::pair<uint32_t, std::string> chat_service_database_api::add_message(const int &sender_id, const int &chat_id, const std::string &message_text) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO messages (sender_id, chat_id, message_text_data) VALUES ($1, $2, $3) RETURNING id, to_char(sended_at, 'YYYY-MM-DD HH24:MI:SS')", sender_id, chat_id, message_text);
    std::pair<uint32_t, std::string> id_timestamp;
    for (auto row : result) {
        id_timestamp = std::make_pair(row[0].as<uint32_t>(), row[1].as<std::string>());
    }
    connection.txn->commit();
    return id_timestamp;
}

void chat_service_database_api::add_unhandle_message(const int &user_id, const int &message_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO unhandled_messages (user_id, message_id) VALUES ($1, $2)", user_id, message_id);
    connection.txn->commit();
}


std::vector<int> chat_service_database_api::get_members_ids(const int &chat_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT user_id FROM chat_membership WHERE chat_id = $1", chat_id);
    std::vector<int> ids;
    ids.reserve(result.size());
    for (auto row : result) {
        ids.push_back(row[0].as<int>());
    }
    connection.txn->commit();
    return ids;
}

user_chats chat_service_database_api::get_user_chats(const int &user_id) {
    const connector connection;
    static std::string query = "SELECT chat_id FROM chat_membership WHERE user_id = $1"; 
    const auto result = connection.txn->exec_params(query, user_id);
    std::vector<uint32_t> chat_ids;
    chat_ids.reserve(result.size());
    for (const auto &row : result) {
        chat_ids.push_back(row[0].as<int>());
    }

    // static constexpr std::string_view query_get_user_chats = 
    // "SELECT c.id, to_char(c.created_at, 'YYYY-MM-DD HH24:MI:SS') AS created_at, c.is_group_chat, c.chat_name, c.description, c.users_count " 
    // "FROM chats AS c "
    // "JOIN chat_membership AS c_m on id = c_m.chat_id "
    // "WHERE c_m.user_id = $1";

    // const auto user_chats_result = connection.txn->exec_params(pqxx::zview(query_get_user_chats), user_id);

    static std::string query_sec = "SELECT id, to_char(created_at, 'YYYY-MM-DD HH24:MI:SS'), is_group_chat, chat_name, description, users_count FROM chats WHERE id = $1";
    user_chats chats;
    chats.user_chats.reserve(chat_ids.size());
    for (const auto &chat_id : chat_ids) {
        chat_preview prewiew;
        const auto result_sec = connection.txn->exec_params(query_sec, chat_id);
        for (const auto &row : result_sec) {
            prewiew.chat_id = row[0].as<int>();
            prewiew.time_stamp = row[1].as<std::string>();
            prewiew.name = row[3].as<std::string>();
            prewiew.description = row[4].as<std::string>();
            static std::string query_third = "SELECT sender_id, to_char(sended_at, 'YYYY-MM-DD HH24:MI:SS'), message_text_data FROM messages WHERE chat_id = $1 ORDER BY sended_at DESC LIMIT 1";
            const auto last_message = connection.txn->exec_params(query_third, chat_id);
            for (const auto &row : last_message) {
                prewiew.last_message.sender = authservice_database_api::get_user_preview(row[0].as<int>());
                prewiew.last_message.time_stamp = row[1].as<std::string>();
                prewiew.last_message.message_text = row[2].as<std::string>();
            }
        }
        chats.user_chats.push_back(prewiew);
    }
    connection.txn->commit();
    return chats;
}

last_messages chat_service_database_api::get_chats_messages(const int &chat_id) {
    const connector connection;
    static std::string query = "SELECT sender_id, to_char(sended_at, 'YYYY-MM-DD HH24:MI:SS'), message_text_data FROM messages WHERE chat_id = $1 ORDER BY sended_at";
    const auto result = connection.txn->exec_params(query, chat_id);
    last_messages messages;
    messages.messages.reserve(result.size());
    for (const auto &row : result) {
        message new_message;
        new_message.sender = authservice_database_api::get_user_preview(row[0].as<int>());
        new_message.time_stamp = row[1].as<std::string>();
        new_message.message_text = row[2].as<std::string>();
        messages.messages.push_back(new_message);
    }
    connection.txn->commit();
    return messages;
}

chat_events chat_service_database_api::get_chat_events(const int &chat_id) {
    const connector connection;
    static std::string query = "SELECT type_of_event, user_id, to_char(time_stamp, 'YYYY-MM-DD HH24:MI:SS'), affected_user FROM chat_events WHERE chat_id = $1 ORDER BY time_stamp";
    const auto result = connection.txn->exec_params(query, chat_id);
    chat_events events;
    events.events.reserve(result.size());
    for (const auto &row : result) {
        chat_event new_event;
        new_event.event = string_to_enum(row[0].as<std::string>());
        if (!row[1].is_null()) {
            new_event.user = authservice_database_api::get_user_preview(row[1].as<int>());
        }
        new_event.time_stamp = row[2].as<std::string>();
        if (!row[3].is_null()) {
            new_event.user_affected = authservice_database_api::get_user_preview(row[2].as<int>());
        }
        events.events.push_back(new_event);
    }
    connection.txn->commit();
    return events;

}

invite::group_chat_preview chat_service_database_api::get_group_chat_preview(const int &chat_id) {
    const connector connection;
    static std::string query = "SELECT id, chat_name, description, cover FROM chats WHERE id = $1";
    const auto result = connection.txn->exec_params(query, chat_id);
    invite::group_chat_preview preview;
    for (const auto &row : result) {
        preview.chat_id = row[0].as<int>();
        preview.name = row[1].as<std::string>();
        if (!row[2].is_null()) {
        preview.description = row[2].as<std::string>();
        }
        if (!row[3].is_null()) {
        preview.chat_cover = row[3].as<std::basic_string<std::byte>>();
        }
    }
    auto chat_members = get_members_ids(chat_id);
    preview.chat_members.reserve(chat_members.size());
    for (const auto &member_id : chat_members) {
        preview.chat_members.push_back(authservice_database_api::get_user_preview(member_id));
    }
    connection.txn->commit();
    return preview;

}

bool chat_service_database_api::check_user_in_chat(const int &user_id, const int &chat_id) {
    const connector connection;
    static std::string query = "SELECT EXISTS(SELECT 1 FROM chat_membership WHERE user_id = $1 AND chat_id = $2)";
    const auto result = connection.txn->exec_params(query, user_id, chat_id);
    bool is_user_in_chat = false;
    for (const auto &row : result) {
        is_user_in_chat = row[0].as<bool>();
    }
    return is_user_in_chat;
}

std::pair<uint32_t, std::string> chat_service_database_api::create_private_chat(const std::pair<int, int> &users_ids)  {
        const connector connection;
        static std::string query = "INSERT INTO chats (is_group_chat, users_count) VALUES (false, 2) RETURNING id, to_char(created_at, 'YYYY-MM-DD HH24:MI:SS')";
        const auto result = connection.txn->exec_params(query);
        std::pair<uint32_t, std::string> id_timestamp;
        for (const auto &row : result) {
            id_timestamp = std::make_pair(row[0].as<int>(), row[1].as<std::string>());
        }
        static std::string query_sec = "INSERT INTO chat_membership (user_id, chat_id) VALUES ($1, $2)";
        connection.txn->exec_params(query_sec, users_ids.first, id_timestamp.first);
        connection.txn->exec_params(query_sec, users_ids.second, id_timestamp.first);
        connection.txn->commit();
        return id_timestamp;
}

std::pair<uint32_t, std::string> chat_service_database_api::create_group_chat(const int &creator_id, const std::string &chat_name, const std::string &description, const std::vector<uint32_t> &users_in_chat) {
    const connector connection;
    static constexpr std::string_view query = "INSERT INTO chats (is_group_chat, users_count, chat_name, description) VALUES (true, $1, $2, $3) RETURNING id, to_char(created_at, 'YYYY-MM-DD HH24:MI:SS')";
    const auto result = connection.txn->exec_params(pqxx::zview(query), users_in_chat.size(), chat_name, description);
    std::pair<uint32_t, std::string> id_timestamp;
    for (const auto &row : result) {
        id_timestamp = std::make_pair(row[0].as<int>(), row[1].as<std::string>());
    }
    static constexpr std::string_view query_sec = "INSERT INTO chat_membership (user_id, chat_id) VALUES ($1, $2)";
    for (const auto &user_id : users_in_chat) {
        connection.txn->exec_params(pqxx::zview(query_sec), user_id, id_timestamp.first);
    }
    static constexpr std::string_view query_third = "INSERT INTO chat_admins (user_id, chat_id) VALUES ($1, $2)";
    connection.txn->exec_params(pqxx::zview(query_third), creator_id, id_timestamp.first);
    connection.txn->commit();
    return id_timestamp;
}

bool chat_service_database_api::check_user_privileges(const int &user_id, const int &chat_id) {
    const connector connection;
    static constexpr std::string_view query = "SELECT EXISTS(SELECT 1 FROM chat_admins WHERE user_id = $1 AND chat_id = $2)";
    const auto result = connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    bool is_user_admin = false;
    for (const auto &row : result) {
        is_user_admin = row[0].as<bool>();
    }
    return is_user_admin;
}

void chat_service_database_api::add_user_to_chat(const int &chat_id, const int &user_id) {
    const connector connection;
    static constexpr std::string_view query = "INSERT INTO chat_membership (user_id, chat_id) VALUES ($1, $2)";
    static constexpr std::string_view update_query = "UPDATE chats SET users_count = users_count + 1 WHERE id = $2";
    static constexpr std::string_view store_chat_event = "INSERT INTO chat_events (type_of_event, chat_id, affected_user) VALUES ('user_entered', $1, $2)";
    connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    connection.txn->exec_params(pqxx::zview(update_query), chat_id);
    connection.txn->exec_params(pqxx::zview(store_chat_event), chat_id, user_id);
    connection.txn->commit();
}

//function to delete user when he leaves chat
void chat_service_database_api::remove_user_from_chat(const int &chat_id, const int &user_id) {
    const connector connection;
    static constexpr std::string_view query = "DELETE FROM chat_membership WHERE user_id = $1 AND chat_id = $2";
    static constexpr std::string_view update_query = "UPDATE chats SET users_count = users_count - 1 WHERE id = $2";
    static constexpr std::string_view store_chat_event = "INSERT INTO chat_events (type_of_event, chat_id, affected_user) VALUES ('user_left', $1, $2)";
    connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    connection.txn->exec_params(pqxx::zview(update_query), chat_id);
    connection.txn->exec_params(pqxx::zview(store_chat_event), chat_id, user_id);
    remove_privileges_from_user(chat_id, user_id); 
    connection.txn->commit();
}

//this overload is for admin to delete user 
void chat_service_database_api::remove_user_from_chat(const int &chat_id, const int &admin_id, const int &user_id_to_remove) {
    const connector connection;
    static constexpr std::string_view query = "DELETE FROM chat_membership WHERE user_id = $1 AND chat_id = $2";
    static constexpr std::string_view update_query = "UPDATE chats SET users_count = users_count - 1 WHERE id = $2";
    static constexpr std::string_view store_chat_event = "INSERT INTO chat_events (type_of_event, chat_id, affected_user) VALUES ('user_removed', $1, $2)";
    connection.txn->exec_params(pqxx::zview(query), user_id_to_remove, chat_id);
    connection.txn->exec_params(pqxx::zview(update_query), chat_id);
    connection.txn->exec_params(pqxx::zview(store_chat_event), chat_id, user_id_to_remove);
    remove_privileges_from_user(chat_id, user_id_to_remove);
    connection.txn->commit();
}

void chat_service_database_api::change_chat_name(const int &chat_id, const int &admin_id, const std::string &name) {
    const connector connection;
    static constexpr std::string_view query = "UPDATE chats SET chat_name = $1 WHERE id = $2";
    static constexpr std::string_view store_chat_event = "INSERT INTO chat_events (type_of_event, chat_id, user_id) VALUES ('name_changed', $1, $2)";
    connection.txn->exec_params(pqxx::zview(query), name, chat_id);
    connection.txn->exec_params(pqxx::zview(store_chat_event), chat_id, admin_id);
    connection.txn->commit();
}

void chat_service_database_api::change_chat_description(const int &chat_id, const int &admin_id, const std::string &description) {
    const connector connection;
    static constexpr std::string_view query = "UPDATE chats SET description = $1 WHERE id = $2";
    static constexpr std::string_view store_chat_event = "INSERT INTO chat_events (type_of_event, chat_id, user_id) VALUES ('description_changed', $1, $2)";
    connection.txn->exec_params(pqxx::zview(query), description, chat_id);
    connection.txn->exec_params(pqxx::zview(store_chat_event), chat_id, admin_id);
    connection.txn->commit();
}

void chat_service_database_api::change_chat_cover(const int &chat_id, const int &admin_id, const std::basic_string<std::byte> &photo_binary) {
    const connector connection;
    static constexpr std::string_view query = "UPDATE chats SET chat_cover = $1 WHERE id = $2";
    static constexpr std::string_view store_chat_event = "INSERT INTO chat_events (type_of_event, chat_id, user_id) VALUES ('cover_changed', $1, $2)";
    connection.txn->exec_params(pqxx::zview(query), photo_binary, chat_id);
    connection.txn->exec_params(pqxx::zview(store_chat_event), chat_id, admin_id);
    connection.txn->commit();
}

void chat_service_database_api::add_privileges_to_user(const int &chat_id, const int &user_id) {
    const connector connection;
    static constexpr std::string_view query = "INSERT INTO chat_admins (user_id, chat_id) VALUES ($1, $2)";
    connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    connection.txn->commit();
}

void chat_service_database_api::remove_privileges_from_user(const int &chat_id, const int &user_id) {
    const connector connection;
    static constexpr std::string_view query = "DELETE FROM chat_admins WHERE user_id = $1 AND chat_id = $2";
    connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    connection.txn->commit();
}

chat_events_types chat_service_database_api::string_to_enum(const std::string &event_type) {
    if (event_type == "user_entered") {
        return chat_events_types::USER_ENTERED;
    } else if (event_type == "user_left") {
        return chat_events_types::USER_LEFT;
    } else if (event_type == "user_removed") {
        return chat_events_types::USER_REMOVED;
    } else if (event_type == "name_changed") {
        return chat_events_types::NAME_CHANGED;
    } else if (event_type == "description_changed") {
        return chat_events_types::DESCRIPTION_CHANGED;
    } else if (event_type == "cover_changed") {
        return chat_events_types::COVER_CHANGED;
    }
    return {};
}

//we innore situation with no admins in chat
//make democracy function to it!!!!!!

}