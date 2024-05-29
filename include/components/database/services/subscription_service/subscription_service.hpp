#pragma once

#include <vector>

namespace database {

    struct subscription_service_database_api {
        [[nodiscard]] static std::vector<uint32_t> get_subscribers(const int &user_id);
        [[nodiscard]] static std::vector<uint32_t> get_subscriptions(const int &user_id);

        [[nodiscard]] static bool check_is_subscribed(const int &subscriber_id, const int &subscribe_to);
        [[nodiscard]] static bool check_is_mutual(const int &subscriber_id, const int &subscribe_to);

        static void subscribe_to_user(const int &subscriber_id, const int &subscribe_to);
        static void unsubscribe_from_user(const int &subscriber_id, const int &subscribe_to);
    };

} // namespace database