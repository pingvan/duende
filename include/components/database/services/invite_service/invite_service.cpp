#include "invite_service.hpp"
#include <grpcpp/support/status.h>
#include <sys/types.h>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include "components/database/connection/database-connector.hpp"
#include "components/database/services/authservice/authservice.hpp"
#include "components/database/services/chat_service/chat_service.hpp"
#include "pqxx/zview.hxx"

namespace database {

std::pair<uint32_t, std::string> invite_service_database_api::add_invite(
    const std::string &invite_text,
    const int &invite_from,
    const int &invite_to
) {
    const connector connection;
    static std::string quer =
        "INSERT INTO invites (invite_text, invite_from_user, invite_to_user) "
        "VALUES ($1, $2, $3) RETURNING id, to_char(sended_at, 'YYYY-MM-DD "
        "HH24:MI:SS')";
    const auto result =
        connection.txn->exec_params(quer, invite_text, invite_from, invite_to);
    std::pair<uint32_t, std::string> id_timestamp;
    for (const auto &row : result) {
        id_timestamp =
            std::make_pair(row[0].as<uint32_t>(), row[1].as<std::string>());
    }
    connection.txn->commit();
    return id_timestamp;
}

std::pair<uint32_t, std::string> invite_service_database_api::add_invite(
    const std::string &invite_text,
    const int &invite_from,
    const int &invite_to,
    const int &chat_id
) {
    const connector connection;
    static std::string quer =
        "INSERT INTO invites (invite_text, invite_from_user, invite_to_user, "
        "chat_id) VALUES ($1, $2, $3, $4) RETURNING id, to_char(sended_at, "
        "'YYYY-MM-DD HH24:MI:SS')";
    const auto result = connection.txn->exec_params(
        quer, invite_text, invite_from, invite_to, chat_id
    );
    std::pair<uint32_t, std::string> id_timestamp;
    for (const auto &row : result) {
        id_timestamp =
            std::make_pair(row[0].as<uint32_t>(), row[1].as<std::string>());
    }
    connection.txn->commit();
    return id_timestamp;
}

void invite_service_database_api::change_invite_status(const int &invite_id) {
    const connector connection;
    static std::string quer_first =
        "DELETE FROM unhandled_invites WHERE invite_id = $1";
    const auto result_first =
        connection.txn->exec_params(quer_first, invite_id);

    static std::string quer_second = "DELETE FROM invites WHERE id = $1";
    const auto result_second =
        connection.txn->exec_params(quer_second, invite_id);
    connection.txn->commit();
}

void invite_service_database_api::add_unhandled_invite(
    const int &invite_to,
    const int &invite_id
) {
    const connector connection;
    static std::string quer =
        "INSERT INTO unhandled_invites (user_id, invite_id) VALUES ($1, $2)";
    const auto result = connection.txn->exec_params(quer, invite_to, invite_id);
    connection.txn->commit();
}

bool invite_service_database_api::is_invite_already_sent(
    const int &invite_from,
    const int &invite_to
) {
    const connector connection;
    static std::string quer =
        "SELECT EXISTS(SELECT id FROM invites WHERE invite_from_user = $1 AND "
        "invite_to_user = $2)";
    const auto result =
        connection.txn->exec_params(quer, invite_from, invite_to);
    bool exists = false;
    for (const auto &row : result) {
        exists = row[0].as<bool>();
    }
    connection.txn->commit();
    return exists;
}

bool invite_service_database_api::is_invite_already_sent_into_the_group_chat(
    const int &chat_id,
    const int &invite_to
) {
    const connector connection;
    static std::string quer =
        "SELECT EXISTS(SELECT id FROM invites WHERE chat_id = $1 AND "
        "invite_to_user = $2)";
    const auto result = connection.txn->exec_params(quer, chat_id, invite_to);
    bool exists = false;
    for (const auto &row : result) {
        exists = row[0].as<bool>();
    }
    connection.txn->commit();
    return exists;
}

bool invite_service_database_api::is_invite_into_group_chat(const int &invite_id
) {
    const connector connection;
    static constexpr std::string_view query =
        "SELECT chat_id FROM invites WHERE chat_id = $1";
    const auto result =
        connection.txn->exec_params(pqxx::zview(query), invite_id);
    bool into_group = false;
    for (const auto &row : result) {
        into_group = !(row[0].is_null());
    }
    return into_group;
}

invites_recivie invite_service_database_api::get_received_invites(
    const int &user_id
) {
    const connector connection;
    static std::string quer =
        "SELECT id, invite_text, invite_from_user, chat_id, to_char(sended_at, "
        "'YYYY-MM-DD HH24:MI:SS') FROM invites WHERE invite_to_user = $1";
    const auto result = connection.txn->exec_params(quer, user_id);
    invites_recivie received;
    received.invites.reserve(result.size());
    for (const auto &row : result) {
        invite new_invite;
        new_invite.invite_id = row[0].as<uint32_t>();
        new_invite.invite_text = row[1].as<std::string>();
        new_invite.sender =
            authservice_database_api::get_user_preview(row[2].as<int>());
        if (!row[3].is_null()) {
            new_invite.chat_preview =
                chat_service_database_api::get_group_chat_preview(
                    row[3].as<int>()
                );
        }
        new_invite.time_stamp = row[4].as<std::string>();
        received.invites.push_back(new_invite);
    }
    connection.txn->commit();
    return received;
}

invites_sent invite_service_database_api::get_sended_invites(const int &user_id
) {
    const connector connection;
    static std::string quer =
        "SELECT id, invite_text, invite_to_user, chat_id, to_char(sended_at, "
        "'YYYY-MM-DD HH24:MI:SS') FROM invites WHERE invite_from_user = $1";
    const auto result = connection.txn->exec_params(quer, user_id);
    invites_sent sended;
    sended.invites.reserve(result.size());
    for (const auto &row : result) {
        invite new_invite;
        new_invite.invite_id = row[0].as<uint32_t>();
        new_invite.invite_text = row[1].as<std::string>();
        new_invite.sender =
            authservice_database_api::get_user_preview(row[2].as<int>());
        if (!row[3].is_null()) {
            new_invite.chat_preview =
                chat_service_database_api::get_group_chat_preview(
                    row[3].as<int>()
                );
        }
        new_invite.time_stamp = row[4].as<std::string>();
        sended.invites.push_back(new_invite);
    }
    connection.txn->commit();
    return sended;
}

}  // namespace database