#include "recommendation_service.hpp"
#include <sstream>
#include <unordered_map>
#include <vector>
#include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"

namespace database {
std::vector<std::vector<int>>
recommendation_service_database_api::getUserRatingMarix() {
    const connector connection;
    static constexpr std::string_view user_count_query =
        "SELECT COUNT(DISTINCT id) FROM users";
    const auto reslut_user_count = connection.txn->exec(user_count_query);
    int user_count = 0;
    for (const auto &row : reslut_user_count) {
        user_count = row[0].as<int>();
    }
    static constexpr std::string_view film_count_query =
        "SELECT COUNT(DISTINCT id) FROM films";
    int film_count = 0;
    const auto reslut_film_count = connection.txn->exec(film_count_query);
    for (const auto &row : reslut_film_count) {
        film_count = row[0].as<int>();
    }
    static constexpr std::string_view query =
        "SELECT ratio, user_id, film_id FROM views";
    const auto result = connection.txn->exec(query);
    std::vector<std::vector<int>> rating_matrix(
        user_count + 1, std::vector<int>(film_count + 1, 0)
    );
    for (const auto &row : result) {
        int ratio = row[0].as<int>();
        int user_id = row[1].as<int>();
        int film_id = row[2].as<int>();
        rating_matrix[user_id][film_id] = ratio;
    }
    connection.txn->commit();
    return rating_matrix;
}

std::vector<std::vector<int>>
recommendation_service_database_api::getUsersRatingsVector(
    const std::vector<int> &users_ids
) {
    const connector connection;
    static constexpr std::string_view get_films_count =
        "SELECT COUNT(DISTINCT id) FROM films";
    const auto films_count =
        connection.txn->query_value<int>(pqxx::zview(get_films_count));
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < users_ids.size(); i++) {
        ss << users_ids[i];
        if (i != users_ids.size() - 1) {
            ss << ',';
        }
    }
    ss << ')';
    std::string query =
        "SELECT ratio, user_id, film_id FROM views WHERE user_id IN " +
        ss.str();
    const auto result = connection.txn->exec(query);
    std::vector<std::vector<int>> rating_matrix(
        users_ids.size(), std::vector<int>(films_count)
    );
    std::size_t i = 0;
    int current_id = -1;
    for (const auto &row : result) {
        int ratio = row[0].as<int>();
        int user_id = row[1].as<int>();
        int film_id = row[2].as<int>();
        if (current_id != user_id) {
            current_id = user_id;
            i++;
        }
        rating_matrix[i - 1][film_id] = ratio;
    }
    connection.txn->commit();
    return rating_matrix;
}
}  // namespace database