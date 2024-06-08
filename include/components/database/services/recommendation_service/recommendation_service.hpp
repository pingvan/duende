#include <pqxx/pqxx>
#include <vector>
#include "components/database/connection/database-connector.hpp"

namespace database {
struct recommendation_service_database_api {
    [[nodiscard]] static std::vector<std::vector<int>> getUserRatingMarix();
    [[nodiscard]] static std::vector<std::vector<int>> getUsersRatingsVector(
        const std::vector<int> &users_ids
    );
};
}  // namespace database