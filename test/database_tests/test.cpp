#include <string>
#include <services/front_service/front_service.hpp>

#include "doctest/doctest.h"

#include "components/database/services/authservice/authservice.hpp"
#include "components/database/services/profile_service/profile_service.hpp"
#include "components/database/services/chat_service/chat_service.hpp"
#include "components/database/services/front_service/front_service.hpp"

std::string email{"test@email.com"};
std::string nickname{"test_nickname"};
std::string token{"not_a_token"};
std::string salt{"testsalt"};
std::string hashed{"hashed_for_test"};

std::string email_for_not_exists{"notexists@email.com"};
std::string nickanem_for_not_exists{"notexists_nickname"};

int id = -1, form_id = -1;

constexpr int UNDEFINED_ID = -1;

using namespace database;

TEST_CASE("add new user") {
    id = authservice::add_user(email, nickname, token, hashed, salt);
    CHECK(id != 0);
}

TEST_CASE("get id of user") {
    CHECK(id == database::authservice::get_user_id(email));
    CHECK(id == database::authservice::get_user_id(nickname));
    CHECK(UNDEFINED_ID == database::authservice::get_user_id(email_for_not_exists));
}

TEST_CASE("check using check fucntions") {
    CHECK(database::authservice::is_email_used(email));
    CHECK(!database::authservice::is_email_used(email_for_not_exists));

    CHECK(authservice::is_nickname_used(nickname));
    CHECK(!authservice::is_nickname_used(nickanem_for_not_exists));
}

TEST_CASE("test hash salt function") {
    auto a = authservice::get_hash_salt(id);
    CHECK(a == std::pair{hashed, salt});
}

TEST_CASE("get refresh token") {
    CHECK(authservice::get_refresh_token(id) == token);
}

TEST_CASE("update refresh token") {
    token = "new_token";
    authservice::update_refresh_token(id, token);
    CHECK(authservice::get_refresh_token(id) == token);
}

TEST_CASE("get form id") {
    form_id = profile_service::get_form_id(id);
    CHECK(form_id != -1);
}

TEST_CASE("change email") {
    email = "new_email@email.com";
    profile_service::change_email(id, email);
    CHECK(authservice::get_user_email(id) == email);
}

TEST_CASE("change nickname") {
    nickname = "new_test_nick";
    profile_service::change_nickname(id, nickname);
    CHECK(authservice::get_user_nickname(id) == nickname);
}

TEST_CASE("check quote") {
    auto quote = "test_quote";
    profile_service::change_quote(id, quote);
}

TEST_CASE("check about") {
    auto about = "test about";
    profile_service::change_about(id, about);
}

TEST_CASE("check add genre") {
    auto nee_genre = "comedy";
    profile_service::add_favourite_genre(id, nee_genre);
}

TEST_CASE("check remove genre") {
    auto to_delete = "comedy";
    profile_service::remove_favourite_genre(id, to_delete);
}

TEST_CASE("check actor") {
    auto actor_id = 10;
    // profile_service::add_favourite_actor(id, actor_id);
}

TEST_CASE("remove fauvorite actor") {

}

TEST_CASE("get user form") {
    front_service::get_user_form(id);
}
