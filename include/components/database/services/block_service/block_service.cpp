#include "components/database/services/block_service/block_service.hpp"
#include <pqxx/pqxx>
#include <string_view>
#include <vector>
#include "components/database/connection/database-connector.hpp"
#include "pqxx/zview.hxx"

namespace database {
std::vector<uint32_t> block_service_database_api::get_blocked_users(
    const int &user_id
) {
    const connector connection;
    static constexpr std::string_view query =
        "SELECT blocked_user_id FROM black_list WHERE user_id = $1";
    const auto result =
        connection.txn->exec_params(pqxx::zview(query), user_id);
    std::vector<uint32_t> blocked_users;
    blocked_users.reserve(result.size());
    for (const auto &row : result) {
        blocked_users.push_back(row[0].as<uint32_t>());
    }
    connection.txn->commit();
    return blocked_users;
}

bool block_service_database_api::check_is_blocked(
    const int &blocker_id,
    const int &blocked_id
) {
    const connector connection;
    static constexpr std::string_view query =
        "SELECT EXISTS(SELECT 1 FROM black_list WHERE user_id = $1 AND "
        "blocked_user_id = $2)";
    const auto result =
        connection.txn->exec_params(pqxx::zview(query), blocker_id, blocked_id);
    connection.txn->commit();
    return result[0][0].as<bool>();
}

void block_service_database_api::block_user(
    const int &blocker_id,
    const int &blocked_id
) {
    const connector connection;
    static constexpr std::string_view query =
        "INSERT INTO black_list (user_id, blocked_user_id) VALUES ($1, $2)";
    const auto result =
        connection.txn->exec_params(pqxx::zview(query), blocker_id, blocked_id);
    connection.txn->commit();
}

void block_service_database_api::unblock_user(
    const int &blocker_id,
    const int &blocked_id
) {
    const connector connection;
    static constexpr std::string_view query =
        "DELETE FROM black_list WHERE user_id = $1 AND blocked_user_id = $2";
    const auto result =
        connection.txn->exec_params(pqxx::zview(query), blocker_id, blocked_id);
    connection.txn->commit();
}
}  // namespace database
