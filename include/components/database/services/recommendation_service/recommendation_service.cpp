#include "recommendation_service.hpp"
#include <unordered_map>
#include <vector>
#include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"

namespace database {
    std::vector<std::vector<int>> recommendation_service_database_api::getUserRatingMarix() {
        const connector connection;
        static std::string user_count_query = "SELECT COUNT(DISTINCT id) FROM users";
        const auto reslut_user_count = connection.txn->exec(user_count_query);
        int user_count = 0;
        for (const auto &row : reslut_user_count) {
            user_count = row[0].as<int>();
        }
        static std::string film_count_query = "SELECT COUNT(DISTINCT id) FROM films";
        int film_count = 0;
        const auto reslut_film_count = connection.txn->exec(film_count_query);
        for (const auto &row : reslut_film_count) {
            film_count = row[0].as<int>();
        }
        static std::string query = "SELECT ratio, user_id, film_id FROM views";
        const auto result = connection.txn->exec(query);
        std::vector<std::vector<int>> rating_matrix(user_count+1, std::vector<int>(film_count+1, 0));
        for (const auto &row : result) {
            int ratio = row[0].as<int>();
            int user_id = row[1].as<int>();
            int film_id = row[2].as<int>();
            rating_matrix[user_id][film_id] = ratio;
        }
        connection.txn->commit();
        return rating_matrix;
    }
}