#include "chat_service.hpp"
#include <components/database/connection/database-connector.hpp>
#include <vector>
#include "components/database/placeholders/placeholders.hpp"
#include "components/database/services/authservice/authservice.hpp"

namespace database {
int chat_service_database_api::add_message(const int &sender_id, const int &chat_id, const std::string &time, const std::string &message_text) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO messages (sender_id, chat_id, sended_at, message_text_data) VALUES ($1, $2, $3, $4) RETURNING id", sender_id, chat_id, time, message_text);
    int message_id = 0;
    for (auto row : result) {
        message_id = row[0].as<int>();
    }
    connection.txn->commit();
    return message_id;
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
    static std::string quer = "SELECT chat_id FROM chat_membership WHERE user_id = $1"; 
    const auto result = connection.txn->exec_params(quer, user_id);
    std::vector<uint> chat_ids;
    chat_ids.reserve(result.size());
    for (const auto &row : result) {
        chat_ids.push_back(row[0].as<int>());
    }
    static std::string quer_sec = "SELECT id, created_at, is_group_chat, chat_name, description, users_count FROM chats WHERE id = $1";
    /*
    SELECT c.chat_id, c.chat_info, m.message_id, m.time_sent
    FROM chats c
    JOIN messages m ON c.chat_id = m.chat_id
    ORDER BY m.time_sent DESC;
    */
    user_chats chats;
    chats.user_chats.reserve(chat_ids.size());
    for (const auto &chat_id : chat_ids) {
        chat_preview prewiew;
        const auto result_sec = connection.txn->exec_params(quer_sec, chat_id);
        for (const auto &row : result_sec) {
            prewiew.chat_id = row[0].as<int>();
            prewiew.time_stamp = row[1].as<std::string>();
            prewiew.name = row[3].as<std::string>();
            prewiew.description = row[4].as<std::string>();
            static std::string quer_third = "SELECT sender_id, sended_at, message_text_data FROM messages WHERE chat_id = $1 ORDER BY sended_at DESC LIMIT 1";
            const auto last_message = connection.txn->exec_params(quer_third, chat_id);
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
    static std::string quer = "SELECT sender_id, sended_at, message_text_data FROM messages WHERE chat_id = $1";
    const auto result = connection.txn->exec_params(quer, chat_id);
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
    static std::string quer = "SELECT EXISTS(SELECT 1 FROM chat_membership WHERE user_id = $1 AND chat_id = $2)";
    const auto result = connection.txn->exec_params(quer, user_id, chat_id);
    bool is_user_in_chat = false;
    for (const auto &row : result) {
        is_user_in_chat = row[0].as<bool>();
    }
    return is_user_in_chat;
}

}