#include "search_service.hpp"
#include <exception>
#include <string>
#include <string_view>
#include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"

namespace database {

std::vector<user_preview>
search_service_database_api::search_user_by_credentials(
    const std::string &user_credentials
) {
    const connector connection;
    static constexpr auto querry =
        "SELECT u.id, u.email, u.login, f.photo "
        "FROM users u "
        "JOIN forms f ON u.id = f.user_id "
        "WHERE u.login ILIKE '%' || $1 || '%' OR u.email ILIKE '%' || $1 || "
        "'%' LIMIT 50";
    const auto result = connection.txn->exec_params(querry, user_credentials);
    std::vector<user_preview> users;
    users.reserve(result.size());
    for (const auto &row : result) {
        if (!row[2].is_null()) {
            users.emplace_back(row[0].as<int>(), row[2].as<std::string>());
        } else {
            users.emplace_back(row[0].as<int>(), row[1].as<std::string>());
        }
        if (!row[3].is_null()) {
            users.back().photo = row[3].as<std::basic_string<std::byte>>();
        }
    }
    return users;
}

std::vector<film_preview> search_service_database_api::search_film_by_title(
    const std::string &title
) {
    const connector connection;
    static constexpr auto querry =
        "SELECT id, title, cover_preview_url FROM films WHERE title ILIKE '%' "
        "|| $1 || '%' LIMIT 50";
    const auto result = connection.txn->exec_params(querry, title);
    std::vector<film_preview> films;
    films.reserve(result.size());
    for (const auto &row : result) {
        films.emplace_back(row[0].as<int>(), row[1].as<std::string>());
        if (!row[2].is_null()) {
            films.back().preview_cover_url = row[2].as<std::string>();
        }
    }
    return films;
}

std::vector<actor_preview> search_service_database_api::search_actor_by_name(
    const std::string &name
) {
    const connector connection;
    static constexpr auto querry =
        "SELECT id, full_name FROM actors WHERE full_name ILIKE '%' || $1 || "
        "'%' LIMIT 50";
    const auto result = connection.txn->exec_params(querry, name);
    std::vector<actor_preview> actors;
    actors.reserve(result.size());
    for (const auto &row : result) {
        actors.emplace_back(row[0].as<int>(), row[1].as<std::string>());
    }
    return actors;
}

}  // namespace database
