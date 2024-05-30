#include "chat_service.hpp"
#include <_types/_uint32_t.h>
#include <components/database/connection/database-connector.hpp>
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
    static std::string query = "SELECT sender_id, to_char(sended_at, 'YYYY-MM-DD HH24:MI:SS'), message_text_data FROM messages WHERE chat_id = $1";
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
    connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    connection.txn->exec_params(pqxx::zview(update_query), chat_id);
    connection.txn->commit();
}

void chat_service_database_api::remove_user_from_chat(const int &chat_id, const int &user_id) {
    const connector connection;
    static constexpr std::string_view query = "DELETE FROM chat_membership WHERE user_id = $1 AND chat_id = $2";
    static constexpr std::string_view update_query = "UPDATE chats SET users_count = users_count - 1 WHERE id = $2";
    connection.txn->exec_params(pqxx::zview(query), user_id, chat_id);
    connection.txn->exec_params(pqxx::zview(update_query), chat_id);
    connection.txn->commit();
}

void chat_service_database_api::change_chat_name(const int &chat_id, const std::string &name) {
    const connector connection;
    static constexpr std::string_view query = "UPDATE chats SET chat_name = $1 WHERE id = $2";
    connection.txn->exec_params(pqxx::zview(query), name, chat_id);
    connection.txn->commit();
}

void chat_service_database_api::change_chat_description(const int &chat_id, const std::string &description) {
    const connector connection;
    static constexpr std::string_view query = "UPDATE chats SET description = $1 WHERE id = $2";
    connection.txn->exec_params(pqxx::zview(query), description, chat_id);
    connection.txn->commit();
}

void chat_service_database_api::change_chat_cover(const int &chat_id, const std::basic_string<std::byte> &photo_binary) {
    const connector connection;
    static constexpr std::string_view query = "UPDATE chats SET chat_cover = $1 WHERE id = $2";
    connection.txn->exec_params(pqxx::zview(query), photo_binary, chat_id);
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

}