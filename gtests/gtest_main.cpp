#define TESTING

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <ios>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <grpcpp/grpcpp.h>

#include "gmock/gmock.h"
#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.hpp"
#include "services/chat_service/chat_service.pb.h"
#include "services/profile_service/profile.grpc.pb.h"
#include "services/profile_service/profile.pb.h"
#include "services/authservice/authservice.grpc.pb.h"
#include "services/authservice/authservice.pb.h"
#include "services/invitation_service/invitation.grpc.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "common/common.pb.h"
#include "common/common.grpc.pb.h"

#include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"
#include "components/database/services/subscription_service/subscription_service.hpp"
#include "components/database/services/profile_service/profile_service.hpp"
#include "components/database/services/authservice/authservice.hpp"

struct databaseTesting : ::testing::Test {
    static const std::string host;

    static const database::connector connector;

    static constexpr uint32_t tablesCNT = 17;  
    static constexpr uint32_t usersCOLUMNS = 3;
    static constexpr uint32_t passwordsCOLUMNS = 3;
    static constexpr uint32_t tokensCOLUMNS = 2;
    static constexpr uint32_t formsCOLUMNS = 6;
    static constexpr uint32_t viewsCOLUMNS = 4;
    static constexpr uint32_t filmsCOLUMNS = 7;
    static constexpr uint32_t producersCOLUMNS = 3;  
    static constexpr uint32_t producers_by_filmCOLUMNS = 2;
    static constexpr uint32_t invitesCOLUMNS = 5;
    static constexpr uint32_t actorsCOLUMNS = 2;
    static constexpr uint32_t actors_to_formCOLUMNS = 2;
    static constexpr uint32_t actors_to_filmCOLUMNS = 2;
    static constexpr uint32_t messagesCOLUMNS = 5;
    static constexpr uint32_t chatsCOLUMNS = 7;
    static constexpr uint32_t chat_membershipCOLUMNS = 2;
};

const std::string databaseTesting::host = "postgres://root:strongpassword@localhost:5432/duende_test";
const database::connector databaseTesting::connector {host};

//think about creating this vecs to any table (not using contains)
// const std::vector<std::string_view> databaseTesting::tables = {
//         "actors",
//         "actors_to_film",
//         "actors_to_form",
//         "chat_membership",
//         "chats",
//         "films",
//         "forms",
//         "invites",
//         "messages",
//         "passwords",
//         "producers",
//         "producers_by_film",
//         "tokens",
//         "unhandled_invites",
//         "unhandled_messages",
//         "users",
//         "views"
// };
struct ProtosAndPlaceholdersTesting : ::testing::Test {
    static const std::basic_string<std::byte> some_image;

    static constexpr uint32_t actor_preview_proto_fields = 2;
    static constexpr uint32_t actor_id = 21; 
    static constexpr std::string_view actor_full_name = "FirstName LastName ThirdName";

    static constexpr uint32_t user_preview_proto_fields = 3;
    static constexpr uint32_t user_id = 21;
    static constexpr std::string_view user_login = "Login";

    static constexpr uint32_t film_preview_proto_fields = 3;
    static constexpr uint32_t film_id = 21;
    static constexpr std::string_view film_title = "Title";
    static constexpr std::string_view film_cover_url = "http://cover.com";

    static constexpr uint32_t film_full_page_proto_fields = 4;
    static constexpr std::string_view film_description = "Description";
    static const std::vector<genre> genres;
    static constexpr std::string_view film_full_page_cover_url = "http://full_cover.com";

    static constexpr uint32_t film_on_user_page_proto_fields = 2;
    static constexpr uint32_t film_ratio = 5;

    static constexpr uint32_t user_full_page_proto_fields = 6;
    static constexpr std::string_view quoete = "Quoete";
    static constexpr std::string_view about = "About";
    static const std::vector<genre> favourite_genres;
    static const std::vector<actor_preview> favourite_actors;
    static const std::vector<film_on_user_page> watched_films;

    static constexpr uint32_t message_proto_fields = 3;
    static constexpr std::string_view message_text = "Text";
    static constexpr std::string_view time_stamp = "12-12-2021 12:12:12";

    static constexpr uint32_t chat_preview_proto_fields = 7;
    static constexpr uint32_t chat_id = 21;
    static constexpr bool is_group_chat = true;
    static constexpr std::string_view chat_name = "Name";
    static constexpr std::string_view chat_description = "Description";
    static constexpr std::string_view chat_time_stamp = "12-12-2021 12:12:12";
    static const std::vector<user_preview> chat_members;

    static constexpr uint32_t user_chats_proto_fields = 1;
    static const std::vector<chat_preview> user_chats_;

    static actor_preview actor_preview_placeholder;
    static user_preview user_preview_placeholder;
    static film_preview film_preview_placeholder;
    static film_full_page film_full_page_placeholder;
    static film_on_user_page film_on_user_page_placeholder;
    static user_full_page user_full_page_placeholder;
    static message message_placeholder;
    static chat_preview chat_preview_placeholder;
    static user_chats user_chats_placeholder;
    static last_messages last_messages_placeholder;
    static invite invite_placeholder;
    static invites_recivie invite_recive_placeholder;
    static invites_sent invites_sent_placeholder;
};

const std::basic_string<std::byte> ProtosAndPlaceholdersTesting::some_image = {std::byte{0x00}, std::byte{0x01}, std::byte{0x02}};

const std::vector<genre> ProtosAndPlaceholdersTesting::genres = {genre::ACTION, genre::COMEDY, genre::DRAMA};
const std::vector<genre> ProtosAndPlaceholdersTesting::favourite_genres = {genre::SCI_FI, genre::HISTORY, genre::DOCUMENTARY};

actor_preview ProtosAndPlaceholdersTesting::actor_preview_placeholder { ProtosAndPlaceholdersTesting::actor_id, std::string(ProtosAndPlaceholdersTesting::actor_full_name) };
user_preview ProtosAndPlaceholdersTesting::user_preview_placeholder { ProtosAndPlaceholdersTesting::user_id, std::string(ProtosAndPlaceholdersTesting::user_login), ProtosAndPlaceholdersTesting::some_image };
film_preview ProtosAndPlaceholdersTesting::film_preview_placeholder { ProtosAndPlaceholdersTesting::film_id, std::string(ProtosAndPlaceholdersTesting::film_title), std::string(ProtosAndPlaceholdersTesting::film_cover_url) };
film_full_page ProtosAndPlaceholdersTesting::film_full_page_placeholder { ProtosAndPlaceholdersTesting::film_preview_placeholder, std::string(ProtosAndPlaceholdersTesting::film_description), ProtosAndPlaceholdersTesting::genres, std::string(ProtosAndPlaceholdersTesting::film_full_page_cover_url) };
film_on_user_page ProtosAndPlaceholdersTesting::film_on_user_page_placeholder { ProtosAndPlaceholdersTesting::film_full_page_placeholder, ProtosAndPlaceholdersTesting::film_ratio };


const std::vector<actor_preview> ProtosAndPlaceholdersTesting::favourite_actors = {ProtosAndPlaceholdersTesting::actor_preview_placeholder};
const std::vector<film_on_user_page> ProtosAndPlaceholdersTesting::watched_films = {ProtosAndPlaceholdersTesting::film_on_user_page_placeholder};

user_full_page ProtosAndPlaceholdersTesting::user_full_page_placeholder { ProtosAndPlaceholdersTesting::user_preview_placeholder, std::string(ProtosAndPlaceholdersTesting::quoete), std::string(ProtosAndPlaceholdersTesting::about), ProtosAndPlaceholdersTesting::favourite_genres, 
ProtosAndPlaceholdersTesting::favourite_actors, ProtosAndPlaceholdersTesting::watched_films };

message ProtosAndPlaceholdersTesting::message_placeholder { std::string(ProtosAndPlaceholdersTesting::message_text), ProtosAndPlaceholdersTesting::user_preview_placeholder, std::string(ProtosAndPlaceholdersTesting::time_stamp) };

const std::vector<user_preview> ProtosAndPlaceholdersTesting::chat_members = {ProtosAndPlaceholdersTesting::user_preview_placeholder};

chat_preview ProtosAndPlaceholdersTesting::chat_preview_placeholder { ProtosAndPlaceholdersTesting::chat_id, ProtosAndPlaceholdersTesting::is_group_chat, std::string(ProtosAndPlaceholdersTesting::chat_name), std::string(ProtosAndPlaceholdersTesting::chat_description), std::string(ProtosAndPlaceholdersTesting::chat_time_stamp),
 ProtosAndPlaceholdersTesting::message_placeholder, ProtosAndPlaceholdersTesting::chat_members, ProtosAndPlaceholdersTesting::some_image };

const std::vector<chat_preview> ProtosAndPlaceholdersTesting::user_chats_ = {ProtosAndPlaceholdersTesting::chat_preview_placeholder};

user_chats ProtosAndPlaceholdersTesting::user_chats_placeholder { ProtosAndPlaceholdersTesting::user_chats_ }; 

template<typename T>
concept WithDescriptor = requires(T proto) { proto.GetDescriptor(); };


template<WithDescriptor T>
[[nodiscard]] uint32_t getFieldsCount(T &proto) {
            const google::protobuf::Descriptor *desc = proto.GetDescriptor();
            return desc->field_count();
}

void check_user_preview(const common::UserPreview &proto, const user_preview &placeholder) {
    EXPECT_EQ(proto.user_id(), placeholder.id);
    EXPECT_EQ(proto.login(), placeholder.login);
    std::string placeholder_photo;
    std::transform(placeholder.photo.begin(), placeholder.photo.end(), std::back_inserter(placeholder_photo), [](std::byte byte) { return static_cast<char>(byte); });
    EXPECT_EQ(proto.photo(), placeholder_photo);
}

void check_actor_preview(const common::ActorPreview &proto, const actor_preview &placeholder) {
    EXPECT_EQ(proto.actor_id(), placeholder.id);
    EXPECT_EQ(proto.full_name(), placeholder.full_name);
}

void check_film_preview(const common::FilmPreview &proto, const film_preview &placeholder) {
    EXPECT_EQ(proto.film_id(), placeholder.id);
    EXPECT_EQ(proto.title(), placeholder.title);
    EXPECT_EQ(proto.preview_cover_url(), placeholder.preview_cover_url);
}

void check_film_full_page(const common::FilmFullPage &proto, const film_full_page &placeholder) {
    check_film_preview(proto.preview(), placeholder.preview);
    EXPECT_EQ(proto.description(), placeholder.description);
    for (int i = 0; i < proto.genres_size(); i++) {
        EXPECT_EQ(proto.genres(i), static_cast<common::Genre>(placeholder.genres[i]));
    }
    EXPECT_EQ(proto.full_page_cover_url(), placeholder.full_page_cover_url);
}

void check_film_on_user_page(const common::FilmOnUserPage &proto, const film_on_user_page &placeholder) {
    check_film_full_page(proto.full_page(), placeholder.full_page);
    EXPECT_EQ(proto.ratio(), placeholder.ratio);
}

void check_user_full_page(const common::UserFullPage &proto, const user_full_page &placeholder) {
    check_user_preview(proto.preview(), placeholder.prewiew);
    EXPECT_EQ(proto.quote(), placeholder.quote);
    EXPECT_EQ(proto.about(), placeholder.about);
    for (int i = 0; i < proto.favorite_genres_size(); i++) {
        EXPECT_EQ(proto.favorite_genres(i), static_cast<common::Genre>(placeholder.fauvorite_genres[i]));
    }
    check_actor_preview(proto.favorite_actors(0), placeholder.fauvorite_actors[0]);
    check_film_on_user_page(proto.watched_films(0), placeholder.watched_films[0]);
}


TEST_F(ProtosAndPlaceholdersTesting, ActorPreview) {
    common::ActorPreview preview_proto;

    const uint32_t proto_fields_by_desc = getFieldsCount(preview_proto);
    auto pl = this->actor_preview_placeholder;
    (this->actor_preview_placeholder).convert_to_proto(&preview_proto);

    EXPECT_EQ(proto_fields_by_desc, this->actor_preview_proto_fields);
    check_actor_preview(preview_proto, this->actor_preview_placeholder);
}

TEST_F(ProtosAndPlaceholdersTesting, UserPreview) {
    common::UserPreview preview_proto;

    const uint32_t proto_fields_by_desc = getFieldsCount(preview_proto);
    auto pl = this->user_preview_placeholder;
    (this->user_preview_placeholder).convert_to_proto(&preview_proto);

    EXPECT_EQ(proto_fields_by_desc, this->user_preview_proto_fields);
    check_user_preview(preview_proto, this->user_preview_placeholder);
}

TEST_F(ProtosAndPlaceholdersTesting, FilmPreview) {
    common::FilmPreview preview_proto;;

    const uint32_t proto_fields_by_desc = getFieldsCount(preview_proto);
    auto pl = this->film_preview_placeholder;
    (this->film_preview_placeholder).convert_to_proto(&preview_proto);

    EXPECT_EQ(proto_fields_by_desc, this->film_preview_proto_fields);
    check_film_preview(preview_proto, this->film_preview_placeholder);
}

TEST_F(ProtosAndPlaceholdersTesting, FilmFullPage) {
    common::FilmFullPage film_full_page_proto;

    const uint32_t proto_fields_by_desc = getFieldsCount(film_full_page_proto);
    auto pl = this->film_full_page_placeholder;
    (this->film_full_page_placeholder).convert_to_proto(&film_full_page_proto);

    EXPECT_EQ(proto_fields_by_desc, this->film_full_page_proto_fields);
    check_film_full_page(film_full_page_proto, this->film_full_page_placeholder);
}

TEST_F(ProtosAndPlaceholdersTesting, FilmOnUserPage) {
    common::FilmOnUserPage film_on_user_page_proto;

    const uint32_t proto_fields_by_desc = getFieldsCount(film_on_user_page_proto);
    auto pl = this->film_on_user_page_placeholder;
    auto pl1 = this->user_full_page_placeholder;
    auto favavec = this->favourite_actors;
    auto watchvec = this->watched_films;
    (this->film_on_user_page_placeholder).convert_to_proto(&film_on_user_page_proto);

    EXPECT_EQ(proto_fields_by_desc, this->film_on_user_page_proto_fields);
    check_film_on_user_page(film_on_user_page_proto, this->film_on_user_page_placeholder);
}

TEST_F(ProtosAndPlaceholdersTesting, UserFullPage) {
    common::UserFullPage user_full_page_proto;

    const uint32_t proto_fields_by_desc = getFieldsCount(user_full_page_proto);
    auto pl = this->user_full_page_placeholder;
    (this->user_full_page_placeholder).convert_to_proto(&user_full_page_proto);

    EXPECT_EQ(proto_fields_by_desc, this->user_full_page_proto_fields);
    check_user_full_page(user_full_page_proto, this->user_full_page_placeholder);
}


TEST_F(databaseTesting, TestConnection) {
    ASSERT_NE(connector.con, nullptr);
    ASSERT_TRUE(connector.con->is_open()) << "Connection to database failed"; 
}

// TEST_F(databaseTesting, TestSubscriptions) {
//     static constexpr std::size_t users_count = 2;
//     static constexpr std::string_view query = "SELECT * FROM users LIMIT 2";
//     const auto result = connector.txn->exec(query);
//     std::vector<uint32_t> ids;
//     ids.reserve(users_count);
//     for (const auto& row : result) {
//         ids.push_back(row[0].as<uint32_t>());
//     }
//     ASSERT_EQ(ids.size(), users_count);
//     auto [first_id, second_id] = std::pair(ids[0], ids[1]);
//     database::subscription_service_database_api::subscribe_to_user(first_id, second_id);
//     ASSERT_TRUE(database::subscription_service_database_api::check_is_subscribed(first_id, second_id));
//     auto subscribers = database::subscription_service_database_api::get_subscribers(second_id);
//     ASSERT_EQ(subscribers.size(), 1);
//     ASSERT_EQ(*(subscribers.begin()), first_id);
//     auto subscriptions = database::subscription_service_database_api::get_subscriptions(first_id);
//     EXPECT_EQ(subscriptions.size(), 1);
//     EXPECT_EQ(*(subscriptions.begin()), second_id);
//     database::subscription_service_database_api::subscribe_to_user(second_id, first_id);
//     ASSERT_TRUE(database::subscription_service_database_api::check_is_subscribed(second_id, first_id));
//     ASSERT_TRUE(database::subscription_service_database_api::check_is_mutual(first_id, second_id));
//     ASSERT_TRUE(database::subscription_service_database_api::check_is_mutual(second_id, first_id));
//     database::subscription_service_database_api::unsubscribe_from_user(first_id, second_id);
//     ASSERT_FALSE(database::subscription_service_database_api::check_is_subscribed(first_id, second_id));
//     ASSERT_FALSE(database::subscription_service_database_api::check_is_mutual(first_id, second_id));
//     ASSERT_FALSE(database::subscription_service_database_api::check_is_mutual(second_id, first_id));
//     auto subscribers_after_unsub = database::subscription_service_database_api::get_subscribers(second_id);
//     ASSERT_TRUE(subscribers_after_unsub.empty());
//     auto subscriptions_after_unsub = database::subscription_service_database_api::get_subscriptions(first_id);
//     ASSERT_TRUE(subscribers_after_unsub.empty());
//     database::subscription_service_database_api::unsubscribe_from_user(second_id, first_id);
//     ASSERT_FALSE(database::subscription_service_database_api::check_is_subscribed(second_id, first_id));
//     ASSERT_FALSE(database::subscription_service_database_api::check_is_mutual(first_id, second_id));
//     ASSERT_FALSE(database::subscription_service_database_api::check_is_mutual(second_id, first_id));
//     auto subscribers_after_unsub_second = database::subscription_service_database_api::get_subscribers(first_id);
//     ASSERT_TRUE(subscribers_after_unsub_second.empty());
//     auto subscriptions_after_unsub_second = database::subscription_service_database_api::get_subscriptions(second_id);
//     ASSERT_TRUE(subscribers_after_unsub_second.empty());
// }

TEST_F(databaseTesting, TestTablesExistence) {
    static constexpr std::string_view quer = "SELECT table_name FROM information_schema.tables WHERE table_schema NOT IN ('pg_catalog', 'information_schema') AND table_type = 'BASE TABLE'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string_view> tables;
    for (const auto& row : result) {
        auto var = row[0].as<std::string_view>();
        ASSERT_EQ(typeid(var), typeid(std::string_view));
        tables.push_back(var);
    }
    EXPECT_EQ(tables.size(), tablesCNT);
    EXPECT_THAT(tables, ::testing::Contains("users"));
    EXPECT_THAT(tables, ::testing::Contains("tokens"));
    EXPECT_THAT(tables, ::testing::Contains("passwords"));
    EXPECT_THAT(tables, ::testing::Contains("forms"));
    EXPECT_THAT(tables, ::testing::Contains("views"));
    EXPECT_THAT(tables, ::testing::Contains("films"));
    EXPECT_THAT(tables, ::testing::Contains("producers"));
    EXPECT_THAT(tables, ::testing::Contains("producers_by_film"));
    EXPECT_THAT(tables, ::testing::Contains("invites"));
    EXPECT_THAT(tables, ::testing::Contains("actors_to_form"));
    EXPECT_THAT(tables, ::testing::Contains("actors"));
    EXPECT_THAT(tables, ::testing::Contains("actors_to_film"));
    EXPECT_THAT(tables, ::testing::Contains("messages"));
    EXPECT_THAT(tables, ::testing::Contains("chats"));
    EXPECT_THAT(tables, ::testing::Contains("chat_membership"));
    EXPECT_THAT(tables, ::testing::Contains("unhandled_invites"));
    EXPECT_THAT(tables, ::testing::Contains("unhandled_messages"));
    EXPECT_THAT(tables, ::testing::Contains("events"));
    EXPECT_THAT(tables, ::testing::Contains("chat_events"));
    EXPECT_THAT(tables, ::testing::Contains("subscriptions"));
    EXPECT_THAT(tables, ::testing::Contains("black_list"));
}


TEST_F(databaseTesting, TestColumnsUsers) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'users'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), usersCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("email"));
    EXPECT_THAT(column_names, ::testing::Contains("login"));
}


TEST_F(databaseTesting, TestColumnsPasswords) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'passwords'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), passwordsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
    EXPECT_THAT(column_names, ::testing::Contains("password_hash"));
    EXPECT_THAT(column_names, ::testing::Contains("password_salt"));
}


TEST_F(databaseTesting, TestColumnsTokens) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'tokens'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), tokensCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
    EXPECT_THAT(column_names, ::testing::Contains("refresh_token"));
}

TEST_F(databaseTesting, TestColumnsForms) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'forms'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), formsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("photo"));
    EXPECT_THAT(column_names, ::testing::Contains("quotation"));
    EXPECT_THAT(column_names, ::testing::Contains("about"));
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
    EXPECT_THAT(column_names, ::testing::Contains("favourite_genres"));
}


//views is a reserved table in postgresql, rewrite
TEST_F(databaseTesting, TestColumnsViews) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'views'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), viewsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("ratio"));
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
    EXPECT_THAT(column_names, ::testing::Contains("film_id"));
}

TEST_F(databaseTesting, TestColumnsFilms) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'films'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), filmsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("title"));
    EXPECT_THAT(column_names, ::testing::Contains("description"));
    EXPECT_THAT(column_names, ::testing::Contains("genres"));
    EXPECT_THAT(column_names, ::testing::Contains("cover_preview_url"));
    EXPECT_THAT(column_names, ::testing::Contains("cover_full_size_url"));
}

TEST_F(databaseTesting, TestColumnsProducers) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'producers'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), producersCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("first_name"));
    EXPECT_THAT(column_names, ::testing::Contains("last_name"));
}

TEST_F(databaseTesting, TestColumnsProducersByFilm) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'producers_by_film'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), producers_by_filmCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("producer_id"));
    EXPECT_THAT(column_names, ::testing::Contains("film_id"));
}

TEST_F(databaseTesting, TestColumnsInvites) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'invites'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), invitesCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_text"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_from_user"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_to_user"));
    EXPECT_THAT(column_names, ::testing::Contains("sended_at"));
}

TEST_F(databaseTesting, TestColumnsActors) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), actorsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("full_name"));
}

TEST_F(databaseTesting, TestColumnsActorsToForm) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors_to_form'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), actors_to_formCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("form_id"));
    EXPECT_THAT(column_names, ::testing::Contains("actor_id"));
}

TEST_F(databaseTesting, TestColumnsActorsToFIlm) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors_to_film'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), actors_to_filmCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("film_id"));
    EXPECT_THAT(column_names, ::testing::Contains("actor_id"));
}

TEST_F(databaseTesting, TestColumnsMessages) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'messages'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), messagesCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("sender_id"));
    EXPECT_THAT(column_names, ::testing::Contains("chat_id"));
    EXPECT_THAT(column_names, ::testing::Contains("sended_at"));
    EXPECT_THAT(column_names, ::testing::Contains("message_text_data"));
}

TEST_F(databaseTesting, TestColumnsChats) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'chats'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), chatsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("created_at"));
    EXPECT_THAT(column_names, ::testing::Contains("is_group_chat"));
    EXPECT_THAT(column_names, ::testing::Contains("chat_name"));
    EXPECT_THAT(column_names, ::testing::Contains("cover"));
    EXPECT_THAT(column_names, ::testing::Contains("description"));
    EXPECT_THAT(column_names, ::testing::Contains("users_count"));
}


TEST_F(databaseTesting, TestColumnsChatMembership) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'chat_membership'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), chat_membershipCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
    EXPECT_THAT(column_names, ::testing::Contains("chat_id"));
}

TEST_F(databaseTesting, TestColumnsUnhandledInvites) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'unhandled_invites'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_id"));
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
}

// struct testingUserActions : ::testing::Test {
//     static uint32_t user_id;
// };

// uint32_t testingUserActions::user_id{};

// TEST_F(testingUserActions, TestAuthService) {
//     static std::string login = "login";
//     static std::string password = "password";
//     static std::string email = "email";
//     static std::string token = "token";
//     static std::string hash = "hash";
//     static std::string salt = "salt";

//     this->user_id = database::authservice_database_api::add_user(email, login, token, hash, salt);
// }

// TEST_F(testingUserActions, TestProfileService) {
//     // genre new_genre = genre::ACTION;
//     std::string new_genre = "action";
//     constexpr uint32_t new_actor_id = 8691;
//     constexpr uint32_t new_film_id = 19995;
//     constexpr uint32_t film_ration = 5;
//     // static constexpr std::string_view new_photo = "new_photo";
//     // database::profile_service_database_api::change_email(user_id, new_email);
//     // database::profile_service_database_api::change_nickname(user_id, new_nickname);
//     // database::profile_service_database_api::change_quotation(user_id, new_quotation);
//     // database::profile_service_database_api::change_about(user_id, new_about);
//     // database::profile_service_database_api::add_favourite_genre(this->user_id, new_genre);
//     // database::profile_service_database_api::remove_favourite_genre(this->user_id, new_genre);
//     // database::profile_service_database_api::add_favourite_actor(user_id, new_actor_id);
//     // database::profile_service_database_api::remove_favourite_actor(8, new_actor_id);
//     // database::profile_service_database_api::add_watched(user_id, new_film_id, film_ration);
//     database::profile_service_database_api::remove_watched(10, new_film_id);
    
// }


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}