#include "invite_service.hpp"
#include "components/database/connection/database-connector.hpp"
#include "components/database/services/authservice/authservice.hpp"


#include <iostream>
#include <string>
#include <unordered_map>


namespace database {
    
    int invite_service_database_api::add_invite(const std::string &invite_text, const invite_status &status, const int &invite_from, const int &invite_to) {
        const connector connection;
        static std::string quer = "INSERT INTO invites (invite_text, invite_status, invite_from_user, invite_to_user) VALUES ($1, $2, $3, $4) RETURNING id";
        const auto result = connection.txn->exec_params(quer, invite_text, enum_invite_status_to_string(status), invite_from, invite_to);
        int invite_id = -1;
        for (const auto &row : result) {
            invite_id = row[0].as<int>();
        }
        connection.txn->commit();
        return invite_id;
    }

    void invite_service_database_api::change_invite_status(const int &invite_id, const invite_status &new_status) {
        const connector connection;
        static std::string quer = "UPDATE invites SET invite_status = $2 WHERE id = $1";
        const auto result = connection.txn->exec_params(quer, invite_id, enum_invite_status_to_string(new_status));
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
        static std::string quer = "SELECT id, invite_text, invite_status, invite_from_user FROM invites WHERE invite_to_user = $1";
        const auto result = connection.txn->exec_params(quer, user_id);
        invites_recivie received;
        received.invites.reserve(result.size());
        for (const auto &row : result) {
            invite new_invite;
            new_invite.invite_id = row[0].as<uint>();
            new_invite.invite_text = row[1].as<std::string>();
            new_invite.status = string_to_invite_status(row[2].as<std::string>());
            new_invite.sender = authservice_database_api::get_user_preview(row[3].as<int>()); //const reference extends the lifetime, right???
            received.invites.push_back(new_invite);
        }
        connection.txn->commit(); 
        return received;
    }

    invites_sent invite_service_database_api::get_sended_invites(const int &user_id) {
        const connector connection;
        static std::string quer = "SELECT id, invite_text, invite_status, invite_to_user FROM invites WHERE invite_from_user = $1";
        const auto result = connection.txn->exec_params(quer, user_id);
        invites_sent sended;
        sended.invites.reserve(result.size());
        for (const auto &row : result) {
            invite new_invite;
            new_invite.invite_id = row[0].as<uint>();
            new_invite.invite_text = row[1].as<std::string>();
            new_invite.status = string_to_invite_status(row[2].as<std::string>());
            new_invite.sender = authservice_database_api::get_user_preview(row[3].as<int>()); //not sender, but...
            sended.invites.push_back(new_invite);
        }
        connection.txn->commit(); 
        return sended;
    }

    std::string invite_service_database_api::enum_invite_status_to_string(const invite_status &status) {
        switch (status) {
            case invite_status::sent: return "sent";
            case invite_status::accepted: return "accepted";
            case invite_status::declined: return "declined";
        }
    }

    invite_status invite_service_database_api::string_to_invite_status(const std::string &status) {
        static const std::unordered_map<std::string, invite_status> status_map = {
        {"sent", invite_status::sent},
        {"accepted", invite_status::accepted},
        {"declined", invite_status::declined}
        };
        if (auto it = status_map.find(status); it != status_map.end()) {
            return it->second;
        } else {
            throw std::exception{};
        }
    }

} //namespace database