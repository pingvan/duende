#include <pqxx/pqxx>

#include <iostream>
#include "components/database/database-connector.hpp"

int main() {
  try {
    int id = database::connector::add_user("ha@email.com", "user", "igfids", "adasgd", "hash", "salt");
    std::cout << database::connector::is_email_used("ha@email.com");
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}