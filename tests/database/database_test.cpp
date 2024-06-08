#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <string_view>
#include <iostream>

#include "components/database/connection/database-connector.hpp"
#include "components/database/placeholders/placeholders.hpp"
#include "components/database/services/subscription_service/subscription_service.hpp"
#include "components/database/services/profile_service/profile_service.hpp"
#include "components/database/services/authservice/authservice.hpp"
#include "components/database/services/chat_service/chat_service.hpp"
#include "components/database/services/invite_service/invite_service.hpp"


struct databaseTesting : ::testing::Test {
    static const std::string host;

    static const database::connector connector;

    static constexpr uint32_t tablesCNT = 23;  
    static constexpr uint32_t usersCOLUMNS = 3;
    static constexpr uint32_t passwordsCOLUMNS = 3;
    static constexpr uint32_t tokensCOLUMNS = 2;
    static constexpr uint32_t formsCOLUMNS = 6;
    static constexpr uint32_t viewsCOLUMNS = 4;
    static constexpr uint32_t filmsCOLUMNS = 7;
    static constexpr uint32_t producersCOLUMNS = 3;  
    static constexpr uint32_t producers_by_filmCOLUMNS = 2;
    static constexpr uint32_t invitesCOLUMNS = 6;
    static constexpr uint32_t actorsCOLUMNS = 2;
    static constexpr uint32_t actors_to_formCOLUMNS = 2;
    static constexpr uint32_t actors_to_filmCOLUMNS = 2;
    static constexpr uint32_t messagesCOLUMNS = 5;
    static constexpr uint32_t chatsCOLUMNS = 7;
    static constexpr uint32_t chat_membershipCOLUMNS = 2;
    static constexpr uint32_t subscriptionsCOLUMNS = 3;
    static constexpr uint32_t black_listCOLUMNS = 2;
    static constexpr uint32_t unhandled_invitesCOLUMNS = 2;
    static constexpr uint32_t unhandled_messagesCOLUMNS = 2;
};

const std::string databaseTesting::host = "postgres://root:strongpassword@localhost:5432/duende";
const database::connector databaseTesting::connector {host};


TEST_F(databaseTesting, TestConnection) {
    ASSERT_NE(connector.con, nullptr);
    ASSERT_TRUE(connector.con->is_open()) << "Connection to database failed"; 
}

TEST_F(databaseTesting, TestTablesExistence) {
    static constexpr std::string_view query = "SELECT table_name FROM information_schema.tables WHERE table_schema NOT IN ('pg_catalog', 'information_schema') AND table_type = 'BASE TABLE'";
    const auto result = connector.txn->exec(query);
    std::vector<std::string> table_names;
    table_names.reserve(result.size());
    for (const auto& row : result) {
        table_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(table_names.size(), tablesCNT);
    EXPECT_THAT(table_names, ::testing::Contains("users"));
    EXPECT_THAT(table_names, ::testing::Contains("tokens"));
    EXPECT_THAT(table_names, ::testing::Contains("passwords"));
    EXPECT_THAT(table_names, ::testing::Contains("forms"));
    EXPECT_THAT(table_names, ::testing::Contains("views"));
    EXPECT_THAT(table_names, ::testing::Contains("films"));
    EXPECT_THAT(table_names, ::testing::Contains("producers"));
    EXPECT_THAT(table_names, ::testing::Contains("producers_by_film"));
    EXPECT_THAT(table_names, ::testing::Contains("invites"));
    EXPECT_THAT(table_names, ::testing::Contains("actors_to_form"));
    EXPECT_THAT(table_names, ::testing::Contains("actors"));
    EXPECT_THAT(table_names, ::testing::Contains("actors_to_film"));
    EXPECT_THAT(table_names, ::testing::Contains("messages"));
    EXPECT_THAT(table_names, ::testing::Contains("chats"));
    EXPECT_THAT(table_names, ::testing::Contains("chat_membership"));
    EXPECT_THAT(table_names, ::testing::Contains("unhandled_invites"));
    EXPECT_THAT(table_names, ::testing::Contains("unhandled_messages"));
    EXPECT_THAT(table_names, ::testing::Contains("chat_events"));
    EXPECT_THAT(table_names, ::testing::Contains("subscriptions"));
    EXPECT_THAT(table_names, ::testing::Contains("black_list"));
    EXPECT_THAT(table_names, ::testing::Contains("chat_admins"));
}


TEST_F(databaseTesting, TestColumnsUsers) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'users'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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


TEST_F(databaseTesting, TestColumnsViews) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'views' AND table_schema = 'public'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), invitesCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("id"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_text"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_from_user"));
    EXPECT_THAT(column_names, ::testing::Contains("invite_to_user"));
    EXPECT_THAT(column_names, ::testing::Contains("sended_at"));
    EXPECT_THAT(column_names, ::testing::Contains("chat_id"));
}

TEST_F(databaseTesting, TestColumnsActors) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
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
    column_names.reserve(result.size());
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), unhandled_invitesCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("invite_id"));
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
}

TEST_F(databaseTesting, TestColumnsUnhandledMessages) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'unhandled_messages'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    column_names.reserve(result.size());
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), unhandled_messagesCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("message_id"));
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
}

TEST_F(databaseTesting, TestColumnsSubscriptions) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'subscriptions'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    column_names.reserve(result.size());
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), subscriptionsCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("subscription_from"));
    EXPECT_THAT(column_names, ::testing::Contains("subscription_to"));
    EXPECT_THAT(column_names, ::testing::Contains("is_mutual"));
}

TEST_F(databaseTesting, TestColumnsBlackList) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'black_list'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    column_names.reserve(result.size());
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    EXPECT_EQ(column_names.size(), black_listCOLUMNS);
    EXPECT_THAT(column_names, ::testing::Contains("user_id"));
    EXPECT_THAT(column_names, ::testing::Contains("blocked_user_id"));
}
