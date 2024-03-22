#include <string>
#include <iostream>
#include "doctest/doctest.h"

#include "services/chat_service/chat_service.hpp"

TEST_CASE("test making timestamp") {
    std::cout << chat_service_::make_timestamp() << '\n';
}