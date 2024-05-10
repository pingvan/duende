#pragma once

#include <vector>

namespace database {

    struct block_service_database_api {
        [[nodiscard]] static std::vector<uint> get_blocked_users(const int &user_id);
        [[nodiscard]] static bool check_is_blocked(const int &blocker_id, const int &blocked_id);
        static void block_user(const int &blocker_id, const int &blocked_id);
    };

} // namespace database