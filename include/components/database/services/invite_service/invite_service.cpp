#include "invite_service.hpp"
#include <sys/types.h>
#include "components/database/connection/database-connector.hpp"
#include "components/database/services/authservice/authservice.hpp"


#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

//treat to enums as int in psql??


namespace database {
    
    std::pair<uint, std::string> invite_service_database_api::add_invite(const std::string &invite_text, const int &invite_from, const int &invite_to) {
        const connector connection;
        static std::string quer = "INSERT INTO invites (invite_text, invite_from_user, invite_to_user) VALUES ($1, $2, $3) RETURNING id, to_char(sended_at, 'YYYY-MM-DD HH24:MI:SS')";
        const auto result = connection.txn->exec_params(quer, invite_text, invite_from, invite_to);
        std::pair<uint, std::string> id_timestamp;
        for (const auto &row : result) {
            id_timestamp = std::make_pair(row[0].as<uint>(), row[1].as<std::string>());
        }
        connection.txn->commit();
        return id_timestamp;
    }

    void invite_service_database_api::change_invite_status(const int &invite_id) {
        const connector connection;
        static std::string quer = "DELETE FORM invites WHERE id = $1";
        const auto result = connection.txn->exec_params(quer, invite_id);
        connection.txn->commit();
    }

    void invite_service_database_api::add_unhandled_invite(const int &invite_to, const int &invite_id) {
        const connector connection;
        static std::string quer = "INSERT INTO unhandled_invites (user_id, invite_id) VALUES ($1, $2)";
        const auto result = connection.txn->exec_params(quer, invite_to, invite_id);
        connection.txn->commit();
    }

    bool invite_service_database_api::is_invite_already_sended(const int &invite_from, const int &invite_to) {
        const connector connection;
        static std::string quer = "SELECT EXISTS(SELECT id FROM invites WHERE invite_from_user = $1 AND invite_to_user = $2)";
        const auto result = connection.txn->exec_params(quer, invite_from, invite_to);
        bool exists = false;
        for (const auto &row : result) {
            exists = row[0].as<bool>();
        }
        connection.txn->commit();
        return exists;
    }

    invites_recivie invite_service_database_api::get_received_invites(const int &user_id) {
        const connector connection;
        static std::string quer = "SELECT id, invite_text, invite_from_user FROM invites WHERE invite_to_user = $1";
        const auto result = connection.txn->exec_params(quer, user_id);
        invites_recivie received;
        received.invites.reserve(result.size());
        for (const auto &row : result) {
            invite new_invite;
            new_invite.invite_id = row[0].as<uint>();
            new_invite.invite_text = row[1].as<std::string>();
            new_invite.sender = authservice_database_api::get_user_preview(row[2].as<int>()); //const reference extends the lifetime, right??? //FIXME
            received.invites.push_back(new_invite);
        }
        connection.txn->commit(); 
        return received;
    }

    invites_sent invite_service_database_api::get_sended_invites(const int &user_id) {
        const connector connection;
        static std::string quer = "SELECT id, invite_text, invite_to_user FROM invites WHERE invite_from_user = $1";
        const auto result = connection.txn->exec_params(quer, user_id);
        invites_sent sended;
        sended.invites.reserve(result.size());
        for (const auto &row : result) {
            invite new_invite;
            new_invite.invite_id = row[0].as<uint>();
            new_invite.invite_text = row[1].as<std::string>();
            new_invite.sender = authservice_database_api::get_user_preview(row[2].as<int>()); //not sender, but... //FIXME
            sended.invites.push_back(new_invite);
        }
        connection.txn->commit(); 
        return sended;
    }

} //namespace database