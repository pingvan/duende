#include <string>
#include <services/front_service/front_service.hpp>
#include <iostream>

// #include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"
#include "doctest/doctest.h"

#include "components/database/services/authservice/authservice.hpp"
#include "components/database/services/profile_service/profile_service.hpp"
#include "components/database/services/chat_service/chat_service.hpp"
#include "components/database/services/front_service/front_service.hpp"
#include "components/database/services/invite_service/invite_service.hpp"

// const database::connector connection;

std::string email{"test@email.com"};
std::string nickname{"test_nickname"};
std::string token{"not_a_token"};
std::string salt{"testsalt"};
std::string hashed{"hashed_for_test"};

std::string email_for_not_exists{"notexists@email.com"};
std::string nickanem_for_not_exists{"notexists_nickname"};

std::string invite_text{"text_of_invite"};

int id = -1, form_id = -1;

constexpr int UNDEFINED_ID = -1;

using namespace database;

//TODO::add grpup test, add environment initialization and clearing data after test
//maybe make another db class for tests

TEST_CASE("add new user") {
    id = authservice_database_api::add_user(email, nickname, token, hashed, salt);
    CHECK(id != UNDEFINED_ID);
}

TEST_CASE("get id of user") {
    CHECK(id == database::authservice_database_api::get_user_id(email));
    CHECK(id == database::authservice_database_api::get_user_id(nickname));
    CHECK(UNDEFINED_ID == database::authservice_database_api::get_user_id(email_for_not_exists));
}

TEST_CASE("check using check fucntions") {
    CHECK(database::authservice_database_api::is_email_used(email));
    CHECK(!database::authservice_database_api::is_email_used(email_for_not_exists));

    CHECK(authservice_database_api::is_nickname_used(nickname));
    CHECK(!authservice_database_api::is_nickname_used(nickanem_for_not_exists));
}

TEST_CASE("test hash salt function") {
    auto a = authservice_database_api::get_hash_salt(id);
    CHECK(a == std::pair{hashed, salt});
}

TEST_CASE("get refresh token") {
    CHECK(authservice_database_api::get_refresh_token(id) == token);
}

TEST_CASE("update refresh token") {
    token = "new_token";
    authservice_database_api::update_refresh_token(id, token);
    CHECK(authservice_database_api::get_refresh_token(id) == token);
}

TEST_CASE("get form id") {
    form_id = profile_service_database_api::get_form_id(id);
    CHECK(form_id != -1);
}

TEST_CASE("change email") {
    email = "new_email@email.com";
    profile_service_database_api::change_email(id, email);
    CHECK(authservice_database_api::get_user_email(id) == email);
}

TEST_CASE("change nickname") {
    nickname = "new_test_nick";
    profile_service_database_api::change_nickname(id, nickname);
    CHECK(authservice_database_api::get_user_nickname(id) == nickname);
}

TEST_CASE("check quote") {
    auto quote = "test_quote";
    profile_service_database_api::change_quotation(id, quote);
}

TEST_CASE("check about") {
    auto about = "test about";
    profile_service_database_api::change_about(id, about);
}

TEST_CASE("check add genre") {
    auto nee_genre = "comedy";
    profile_service_database_api::add_favourite_genre(id, nee_genre);
}

TEST_CASE("check remove genre") {
    auto to_delete = "comedy";
    profile_service_database_api::remove_favourite_genre(id, to_delete);
}

TEST_CASE("check actor") {
    // auto actor_id = 10;
    // profile_service::add_favourite_actor(id, actor_id);
}

TEST_CASE("remove fauvorite actor") {

}

TEST_CASE("get user form") {
    // front_service::get_user_form(id);
}

TEST_CASE("testing getting full user_page") {
    user_full_page full_page = authservice_database_api::get_user_full_page(1);
    // std::cerr << "login: " << full_page.prewiew.login << '\n';
    // std::cerr << "quote: " << full_page.quote << '\n';
    // std::cerr << "about: "<< full_page.about << '\n';
    // std::cerr << "fauvorite genres: " << '\n';
    // std::cerr << "actors: " << '\n';
    for (const auto &actor : full_page.fauvorite_actors) {
        // std::cerr << actor.full_name << '\n';
    }
    // std::cerr << "watched films: " << '\n';
    for (const auto &film : full_page.watched_films) {
        // std::cerr << film.full_page.preview.title << ' ' << 
        // film.ratio << ' '  << '\n';
    }
}

TEST_CASE("get user chats") {
    // connection.execute(const std::string &queiry, Args args...)
}

TEST_CASE("get last messages") {

}


TEST_CASE("testing invite sending") {
    const std::string invite_text = "text_of_invite";
    const std::string email1 = "email1";
    const std::string email2 = "email2";
    const std::string login1 = "login1";
    const std::string login2 = "login2";
    auto id1 = authservice_database_api::add_user(email1, login1, "token1", "hash1", "salt1");
    REQUIRE(id1 != UNDEFINED_ID);
    auto id2 = authservice_database_api::add_user(email2, login2, "token2", "hash2", "salt2");
    REQUIRE(id2 != UNDEFINED_ID);
    auto invite_id = invite_service_database_api::add_invite(invite_text, invite_status::sent, id1, id2);
    REQUIRE(invite_id != UNDEFINED_ID);
    invites_recivie recieved = invite_service_database_api::get_received_invites(id2);
    REQUIRE(recieved.invites.size() != 0);
    auto &first_invite_received = recieved.invites[0];
    CHECK(first_invite_received.invite_id == invite_id);
    CHECK(first_invite_received.invite_text == invite_text);
    CHECK(first_invite_received.sender.login == login1);
    CHECK(first_invite_received.status == invite_status::sent);

    invites_sent sended = invite_service_database_api::get_sended_invites(id1);
    REQUIRE(sended.invites.size() != 0);
    auto &first_invite_sent = sended.invites[0];
    CHECK(first_invite_sent.invite_id == invite_id);
    CHECK(first_invite_sent.invite_text == invite_text);
    CHECK(first_invite_sent.sender.login == login2);
    CHECK(first_invite_sent.status == invite_status::sent);
    
}

TEST_CASE("testing unhandeling invites") {

}

TEST_CASE("test getting invites") {

}

