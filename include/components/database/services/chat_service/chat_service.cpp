#include "chat_service.hpp"
#include <components/database/connection/database-connector.hpp>

namespace database {
int chat_service::add_message(const int &sender_id, const int &chat_id, const std::string &time, const std::string &message_text) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO messages (sender_id, chat_id, time, message_text) VALUES ($1, $2, $3, $4) RETURNING id", sender_id, chat_id, time, message_text);
    int message_id = 0;
    for (auto row : result) {
        message_id = row[0].as<int>();
    }
    connection.txn->commit();
    return message_id;
}

void chat_service::add_unhandle_message(const int &user_id, const int &message_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("INSERT INTO unhandled_messages (user_id, message_id) VALUES ($1, $2)", user_id, message_id);
    connection.txn->commit();
}


std::vector<int> chat_service::get_members_ids(const int &chat_id) {
    const connector connection;
    const auto result = connection.txn->exec_params("SELECT user_id FROM chat_membership WHERE chat_id = $1", chat_id);
    std::vector<int> ids;
    ids.reserve(result.size());
    for (auto row : result) {
        ids.push_back(row[0].as<int>());
    }
    return ids;
}
}