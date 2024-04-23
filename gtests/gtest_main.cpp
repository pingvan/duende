#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cassert>
#include <memory>
#include <string_view>
#include <vector>

#include "components/database/connection/database-connector.hpp"

struct databaseTesting : ::testing::Test {
    void SetUp() override {
        // connector = std::make_unique<database::connector>();

    }

    void TearDown() override {
        for (const auto& quer : clearing_commands) {
            std::cerr << quer << std::endl;
            // connector.txn->exec(quer);
        }
        // connector.reset();
    }

    // std::unique_ptr<database::connector> connector;
    database::connector connector;

    static const std::vector<std::string_view> tables;

    static std::vector<std::string_view> clearing_commands;

    const uint tablesCNT = 17;  //should be 17 if unhandeled tables are added to the database
    const uint usersCOLUMNS = 3;
    const uint passwordsCOLUMNS = 3;
    const uint tokensCOLUMNS = 2;
    const uint formsCOLUMNS = 6;
    const uint viewsCOLUMNS = 4;
    const uint filmsCOLUMNS = 4;
    const uint producersCOLUMNS = 3;  //i think will be 2 in future
    const uint producers_by_filmCOLUMNS = 2;
    const uint invitesCOLUMNS = 5;
    const uint actorsCOLUMNS = 2;
    const uint actors_to_formCOLUMNS = 2;
    const uint actors_to_filmCOLUMNS = 2;
    const uint messagesCOLUMNS = 5;
    const uint chatsCOLUMNS = 6;
    const uint chat_membershipCOLUMNS = 3;
};

//think about creating this vecs to any table (not using contains)
const std::vector<std::string_view> databaseTesting::tables = {
        "actors",
        "actors_to_film",
        "actors_to_form",
        "chat_membership",
        "chats",
        "films",
        "forms",
        "invites",
        "messages",
        "passwords",
        "producers",
        "producers_by_film",
        "tokens",
        "unhandled_invites",
        "unhandled_messages",
        "users",
        "views"
};

std::vector<std::string_view> databaseTesting::clearing_commands = {};

TEST_F(databaseTesting, TestConnection) {
    ASSERT_NE(connector.con, nullptr);
    ASSERT_TRUE(connector.con->is_open()) << "Connection to database failed"; 
}

TEST_F(databaseTesting, TestTablesExistence) {
    static constexpr std::string_view quer = "SELECT table_name FROM information_schema.tables WHERE table_schema NOT IN ('pg_catalog', 'information_schema') AND table_type = 'BASE TABLE'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string_view> tables;
    for (const auto& row : result) {
        auto var = row[0].as<std::string_view>();
        ASSERT_EQ(typeid(var), typeid(std::string_view));
        tables.push_back(var);
    }
    std::sort(tables.begin(), tables.end());
    ASSERT_EQ(tables, this->tables);
    //change to expect

    this->clearing_commands.push_back("first");
    ASSERT_EQ(tables.size(), tablesCNT);
    ASSERT_THAT(tables, ::testing::Contains("users"));
    ASSERT_THAT(tables, ::testing::Contains("tokens"));
    ASSERT_THAT(tables, ::testing::Contains("passwords"));
    ASSERT_THAT(tables, ::testing::Contains("forms"));
    ASSERT_THAT(tables, ::testing::Contains("views"));
    ASSERT_THAT(tables, ::testing::Contains("films"));
    ASSERT_THAT(tables, ::testing::Contains("producers"));
    ASSERT_THAT(tables, ::testing::Contains("producers_by_film"));
    ASSERT_THAT(tables, ::testing::Contains("invites"));
    ASSERT_THAT(tables, ::testing::Contains("actors_to_form"));
    ASSERT_THAT(tables, ::testing::Contains("actors"));
    ASSERT_THAT(tables, ::testing::Contains("actors_to_film"));
    ASSERT_THAT(tables, ::testing::Contains("messages"));
    ASSERT_THAT(tables, ::testing::Contains("chats"));
    ASSERT_THAT(tables, ::testing::Contains("chat_membership"));
}


TEST_F(databaseTesting, TestColumnsUsers) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'users'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }

    this->clearing_commands.push_back("second");
    ASSERT_EQ(column_names.size(), usersCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("email"));
    ASSERT_THAT(column_names, ::testing::Contains("login"));
}


TEST_F(databaseTesting, TestColumnsPasswords) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'passwords'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }

    this->clearing_commands.push_back("third");
    ASSERT_EQ(column_names.size(), passwordsCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("user_id"));
    ASSERT_THAT(column_names, ::testing::Contains("password_hash"));
    ASSERT_THAT(column_names, ::testing::Contains("password_salt"));
}


TEST_F(databaseTesting, TestColumnsTokens) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'tokens'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), tokensCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("user_id"));
    ASSERT_THAT(column_names, ::testing::Contains("refresh_token"));
}

TEST_F(databaseTesting, TestColumnsForms) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'forms'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    // ASSERT_EQ(column_names.size(), formsCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    // ASSERT_THAT(column_names, ::testing::Contains("photo"));
    ASSERT_THAT(column_names, ::testing::Contains("quotation"));
    ASSERT_THAT(column_names, ::testing::Contains("about"));
    ASSERT_THAT(column_names, ::testing::Contains("user_id"));
    ASSERT_THAT(column_names, ::testing::Contains("favourite_genres"));
}


//views is a reserved table in postgresql, rewrite
TEST_F(databaseTesting, TestColumnsViews) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'views'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    // ASSERT_EQ(column_names.size(), viewsCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("ratio"));
    ASSERT_THAT(column_names, ::testing::Contains("user_id"));
    ASSERT_THAT(column_names, ::testing::Contains("film_id"));
}

TEST_F(databaseTesting, TestColumnsFilms) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'films'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), filmsCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("title"));
    ASSERT_THAT(column_names, ::testing::Contains("description"));
    ASSERT_THAT(column_names, ::testing::Contains("genres"));
}

TEST_F(databaseTesting, TestColumnsProducers) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'producers'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), producersCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("first_name"));
    ASSERT_THAT(column_names, ::testing::Contains("last_name"));
}

TEST_F(databaseTesting, TestColumnsProducersByFilm) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'producers_by_film'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), producers_by_filmCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("producer_id"));
    ASSERT_THAT(column_names, ::testing::Contains("film_id"));
}

TEST_F(databaseTesting, TestColumnsInvites) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'invites'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), invitesCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("invite_text"));
    ASSERT_THAT(column_names, ::testing::Contains("invite_status"));
    ASSERT_THAT(column_names, ::testing::Contains("invite_from_user"));
    ASSERT_THAT(column_names, ::testing::Contains("invite_to_user"));
}

TEST_F(databaseTesting, TestColumnsActors) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), actorsCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("full_name"));
}

TEST_F(databaseTesting, TestColumnsActorsToForm) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors_to_form'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), actors_to_formCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("form_id"));
    ASSERT_THAT(column_names, ::testing::Contains("actor_id"));
}

TEST_F(databaseTesting, TestColumnsActorsToFIlm) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'actors_to_film'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), actors_to_filmCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("film_id"));
    ASSERT_THAT(column_names, ::testing::Contains("actor_id"));
}

TEST_F(databaseTesting, TestColumnsMessages) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'messages'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), messagesCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("sender_id"));
    ASSERT_THAT(column_names, ::testing::Contains("chat_id"));
    ASSERT_THAT(column_names, ::testing::Contains("sended_at"));
    ASSERT_THAT(column_names, ::testing::Contains("message_text_data"));
}

TEST_F(databaseTesting, TestColumnsChats) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'chats'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    ASSERT_EQ(column_names.size(), chatsCOLUMNS);
    ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("created_at"));
    ASSERT_THAT(column_names, ::testing::Contains("is_group_chat"));
    ASSERT_THAT(column_names, ::testing::Contains("chat_name"));
    ASSERT_THAT(column_names, ::testing::Contains("description"));
    ASSERT_THAT(column_names, ::testing::Contains("users_count"));
}


TEST_F(databaseTesting, TestColumnsChatMembership) {
    static constexpr std::string_view quer = "SELECT column_name FROM information_schema.columns WHERE table_name = 'chat_membership'";
    auto result = connector.txn->exec(quer);
    std::vector<std::string> column_names;
    for (const auto& row : result) {
        column_names.push_back(row[0].as<std::string>());
    }
    // ASSERT_EQ(column_names.size(), chat_membershipCOLUMNS);
    // ASSERT_THAT(column_names, ::testing::Contains("id"));
    ASSERT_THAT(column_names, ::testing::Contains("user_id"));
    ASSERT_THAT(column_names, ::testing::Contains("chat_id"));
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}