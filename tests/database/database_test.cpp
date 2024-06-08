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

// std::vector<char> read_photo_as_binary(const std::filesystem::path &path) {  
//     std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
//     // std::ifstream file;
//     // file.open(path, std::ios_base::in | std::ios_base::binary);
//     if (!file.is_open()) {
//         std::cerr << "Can't open photo file\n";
//         // std::exit(1);
//         return {};
//     }
//     std::vector<char> vector_binary_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());;
//     // std::basic_string<std::byte> string_binary_data;
//     // std::string string_binary_data;
//     // std::transform(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(vector_binary_data), [](char char_) { return static_cast<std::byte>(char_); });
//     return vector_binary_data;
//     // std::transform(string_binary_data.begin(), string_binary_data.end(), std::back_inserter(string_binary_data), [](char char_) { return static_cast<std::byte>(char_); });
//     // std::transform(vector_binary_data.begin(), vector_binary_data.end(), std::back_inserter(vector_binary_data), )
// }

// void write_photo_from_binary(const std::string &photo_binaty) {
//     std::ofstream out("../gtests/test_out.jpg", std::ios_base::out | std::ios_base::binary);
//     if (!out.is_open()) {
//         std::cerr << "can't open out\n";
//         // std::exit(1);
//     }
//     out.write(photo_binaty.data(), photo_binaty.size());
//     out.close();
// }

// void write_photo_binary(const std::string &photo_name, const std::basic_string<std::byte> &ph) {
//     std::ofstream out("../gtests/" + photo_name + ".jpg", std::ios_base::out | std::ios_base::binary);
//     if (!out.is_open()) {
//         std::cerr << "can't open out\n";
//         // std::exit(1);
//     }
//     out.write(reinterpret_cast<const char*>(ph.c_str()), ph.size());
//     out.close();
// }

// TEST_F(databaseTesting, TestChangingPhoto) {
//     static std::string server_address = "0.0.0.0:50051";
//     auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
//     auto stub = profile::ProfileService::NewStub(channel);
//     grpc::ClientContext context;
//     profile::ProfilePhotoRequest request;
//     profile::ProfileReply response;
//     std::cout << "Current path: " << std::filesystem::current_path() << '\n';
//     auto res_binary = read_photo_as_binary("../gtests/test_in.jpg");
//     std::cout << "size of image: " << res_binary.size() << std::endl;
//     // std::cerr << "Binary vector: " << std::endl;
//     // for (const auto b : res_binary) {
//     //     std::cout << (char)b;
//     // }
//     request.set_user_id(1);
//     std::string string_binary;
//     std::transform(res_binary.begin(), res_binary.end(), std::back_inserter(string_binary), [](char byte_) { return static_cast<char>(byte_); } );
//     // std::cerr << "Binary photo to insert: " << string_binary << std::endl;
//     std::cout << "size of image: " << string_binary.size() << std::endl;
//     request.set_photo(string_binary);
//     std::cout << request.ByteSizeLong() << '\n';
//     auto status = stub->ChangeProfilePhoto(&context, request, &response);
//     if (!status.ok()) {
//         std::cerr << "No\n";
//     }

//     static constexpr std::string_view query = "SELECT photo FROM forms WHERE user_id = $1";
//     auto res = connector.txn->exec_params(pqxx::zview(query), 1);
//     std::basic_string<std::byte> photo_from_db;
//     for (const auto &row : res) {
//         if (!row[0].is_null()) {
//             photo_from_db = row[0].as<std::basic_string<std::byte>>();
//             std::cerr << "Get photo\n";
//         }
//     }
//     // std::string out_str;
//     // std::transform(photo_from_db.begin(), photo_from_db.end(), std::back_inserter(out_str), [](std::byte byte_){return static_cast<char>(byte_);});
//     // write_photo_from_binary(out_str);
// }

// TEST_F(databaseTesting, GetPhotoFromProfile) {
//     static constexpr std::string_view query = "SELECT photo FROM forms WHERE user_id = $1";
//     pqxx::result res;
//     try {
//      res = connector.txn->exec_params(pqxx::zview(query), 1);
//     } catch(const std::exception &e) {
//         std::cerr << e.what() << std::endl;
//     }
//     std::cerr << res.affected_rows() << ' ' << res.size() << std::endl;
//     std::basic_string<std::byte> photo_from_db;
//     std::string out_ff;
//     // pqxx::binarystring ss_binary;
//     for (const auto &row : res) {
//         // if (!row["photo"].is_null()) {
//         // std::cout << row["photo"].as<std::string>();
//         // }
//         if (!row[0].is_null()) {
//             photo_from_db = row[0].as<std::basic_string<std::byte>>();
//             out_ff = row[0].as<std::string>();
//             auto as_bs = row[0].as<pqxx::binarystring>();
//             std::cerr << "Get photo\n";
//             std::cerr << "as bs: " << as_bs.size() << std::endl;
//         }
//     }
//     std::string out_str;
//     std::cout << "size of ph: " << out_ff.size() << '\n';
//     std::cout << photo_from_db.size() << ' ';
//     // std::transform(photo_from_db.begin(), photo_from_db.end(), std::back_inserter(out_str), [](std::byte byte_){return static_cast<char>(byte_);});
//     // std::cout << out_str << '\n';
//     // std::cout << out_ff << '\n';
//     // write_photo_from_binary(out_ff);
//     write_photo_binary(photo_from_db);
// }

// TEST_F(databaseTesting, getfilm) {
//     for (int i = 1; i < 10; i++) {
//     auto res = database::profile_service_database_api::get_film_full_page(i);
//     auto film_full_page_proto = new common::FilmFullPage;
//     res.convert_to_proto(film_full_page_proto);
//     std::cerr << film_full_page_proto->DebugString() << std::endl;
//     }
// }


// TEST_F(databaseTesting, getchatprev) {
//     auto result = database::chat_service_database_api::get_chat_preview(1);
//     auto preview_proto = new chat_service::ChatPreview;
//     result.convert_to_proto(preview_proto);
//     std::cerr << preview_proto->DebugString() << std::endl;
// }

// TEST_F(databaseTesting, checkg) {
//     auto result = database::invite_service_database_api::is_invite_into_group_chat(1);
// }