#include "subscription_service.hpp"
#include <pqxx/pqxx>
#include <string_view>
#include <vector>
#include <string>
#include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"

namespace database {
    
    std::vector<uint32_t> subscription_service_database_api::get_subscribers(const int &user_id) {
        const connector connection;
        static  std::string query = "SELECT subscription_from FROM subscriptions WHERE subscription_to = $1";
        const auto result = connection.txn->exec_params( (query), user_id);
        std::vector<uint32_t> subscribers_ids;
        subscribers_ids.reserve(result.size());
        for (const auto &row : result) {
            subscribers_ids.push_back(row[0].as<uint32_t>()); 
        }
        connection.txn->commit();
        return subscribers_ids;
    }

    std::vector<uint32_t> subscription_service_database_api::get_subscriptions(const int &user_id) {
        const connector connection;
        static  std::string query = "SELECT subscription_to FROM subscriptions WHERE subscription_from = $1";
        const auto result = connection.txn->exec_params( (query), user_id);
        std::vector<uint32_t> subscriptions_ids;
        subscriptions_ids.reserve(result.size());
        for (const auto &row : result) {
            subscriptions_ids.push_back(row[0].as<uint32_t>());
        }
        connection.txn->commit();
        return subscriptions_ids;
    }

    bool subscription_service_database_api::check_is_subscribed(const int &subscriber_id, const int &subscribe_to) {
        const connector connection;
        static  std::string query = "SELECT EXISTS(SELECT 1 FROM subscriptions WHERE subscription_from = $1 AND subscription_to = $2)";
        const auto result = connection.txn->exec_params( (query), subscriber_id, subscribe_to);
        bool exists = false;
        for (const auto &row : result) {
            exists = row[0].as<bool>();
        }
        connection.txn->commit();
        return exists;
    }

    bool subscription_service_database_api::check_is_mutual(const int &subscriber_id, const int &subscribe_to) {
        const connector connection;
        static  std::string query = "SELECT is_mutual FROM subscriptions WHERE subscription_from = $1 AND subscription_to = $2";
        const auto result = connection.txn->exec_params( (query), subscriber_id, subscribe_to);
        bool is_mutual = false;
        for (const auto &row : result) {
            is_mutual = row[0].as<bool>();
        }
        connection.txn->commit();
        return is_mutual;
    }

    void subscription_service_database_api::subscribe_to_user(const int &subscriber_id, const int &subscribe_to) {
        const connector connection;
        bool is_subscribed = check_is_subscribed(subscribe_to, subscriber_id);
        if (is_subscribed) {
            static  std::string query = "UPDATE subscriptions SET is_mutual = true WHERE subscription_from = $1 AND subscription_to = $2";
            const auto result_second = connection.txn->exec_params( (query), subscribe_to, subscriber_id);
        }
        static  std::string query = "INSERT INTO subscriptions (subscription_from, subscription_to, is_mutual) VALUES ($1, $2, $3)";
        const auto result = connection.txn->exec_params( (query), subscriber_id, subscribe_to, is_subscribed);
        connection.txn->commit();
    }

    void subscription_service_database_api::unsubscribe_from_user(const int &subscriber_id, const int &subscribe_to) {
        const connector connection;
        bool is_subscribed = check_is_subscribed(subscribe_to, subscriber_id);
        if (is_subscribed) {
            static std::string query = "UPDATE subscriptions SET is_mutual = false WHERE subscription_to = $1 AND subscription_from = $2";
            const auto result_second = connection.txn->exec_params( (query), subscriber_id, subscribe_to);
        }
        static  std::string query = "DELETE FROM subscriptions WHERE subscription_from = $1 AND subscription_to = $2";
        const auto result = connection.txn->exec_params( (query), subscriber_id, subscribe_to);
        connection.txn->commit();
    }

}