#pragma once

#include <vector>
#include <string>
#include "components/database/placeholders/placeholders.hpp"

namespace database {

struct search_service_database_api {
    static std::vector<user_preview> search_user_by_credentials(const std::string& user_credentials);
    static std::vector<film_preview> search_film_by_title(const std::string& title);
    static std::vector<actor_preview> search_actor_by_name(const std::string& name);
};

} // namespace database