#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>
#include <vector>
#include <grpcpp/grpcpp.h>

#include "services/chat_service/chat_service.grpc.pb.h"
#include "services/chat_service/chat_service.pb.h"
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

TEST(chat_service, TestChatService) {
    std::mutex m;
    std::condition_variable cv;
    std::condition_variable cv_ready_for_second;
    bool ready = false, ready_for_second = false;

    std::thread first_client([&]() {

        auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
        auto stub = chat_service::ChatHandler::NewStub(channel);
        auto context = new grpc::ClientContext();
        // context->set_deadline(std::chrono::milliseconds(1000));
        auto stream = stub->HandleChat(context);
        chat_service::ChatRequests request;

        std::thread ([&]() {
            chat_service::ChatResponses response;
            while (stream->Read(&response)) {
                if (response.has_accept_message()) {
                    std::cout << "Message accepted" << std::endl;
                }
            }
            std::cout << "reader first finished" << std::endl;
        }).detach();

        request.mutable_init()->set_user_id(64);
        stream->Write(request);

        std::unique_lock<std::mutex> l(m);
        ready = true;
        cv.notify_all();
        l.unlock();
        std::cout << "second client notified\n";

        chat_service::ChatRequests request2;
        request2.mutable_send_message()->mutable_msg()->set_sender_id(64);
        request2.mutable_send_message()->mutable_msg()->set_chat_id(1);
        request2.mutable_send_message()->mutable_msg()->set_message_text("Hello");
        stream->Write(request2);

        std::unique_lock<std::mutex> l1(m);
        cv_ready_for_second.wait(l1, [&](){ return ready_for_second; });
        l1.unlock();

        chat_service::ChatRequests request3;
        request2.mutable_send_message()->mutable_msg()->set_sender_id(64);
        request2.mutable_send_message()->mutable_msg()->set_chat_id(1);
        request2.mutable_send_message()->mutable_msg()->set_message_text("Word");
        stream->Write(request2);
        std::cout << "wrote second msg\n";

        std::this_thread::sleep_for(std::chrono::seconds(2));

        // stream->Finish();
        stream->WritesDone();
        std::cout << "first finished\n";
    });

    std::thread second_client([&]() {
        auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
        auto stub = chat_service::ChatHandler::NewStub(channel);
        auto context = new grpc::ClientContext();
        // context->set_deadline(std::chrono::milliseconds(1000));
        auto stream = stub->HandleChat(context);
        chat_service::ChatRequests request;

        std::mutex local_m;
        std::condition_variable cv_get_first;
        bool get_first = false;

        bool end_reading = false;
        std::thread ([&]() {
            chat_service::ChatResponses response;
            while (stream->Read(&response)) {
                if (response.has_accept_message()) {
                    EXPECT_EQ(response.accept_message().msg().sender_id(), 64);
                    EXPECT_EQ(response.accept_message().msg().chat_id(), 1);
                    EXPECT_EQ(response.accept_message().msg().message_text(), "Hello");
                    std::cout << "Message accepted from 2nd" << std::endl;

                    std::unique_lock<std::mutex> l(local_m);
                    get_first = true;
                    cv_get_first.notify_all();
                    std::cout << "second notified about readig complette\n";
                    l.unlock();
                }
            }
            // end_reading = true;
            std::cout << "reader second finished" << std::endl;
        }).detach();

        request.mutable_init()->set_user_id(71);
        stream->Write(request);

        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [&](){return ready;});
        l.unlock();
        std::cout << "both threads inited\n";

        std::unique_lock<std::mutex> l1(local_m);
        cv_get_first.wait(l1, [&](){return get_first;});
        std::cout << "first message readed, finishinf stream\n";
        l1.unlock();
        std::cout << "unlocked\n";
        stream->WritesDone();
        // stream->Finish();
        std::cout << "second stream finished\n";

        // ASSERT_TRUE(end_reading);
        std::unique_lock<std::mutex> l2(m);
        ready_for_second = true;
        cv_ready_for_second.notify_all();
        l2.unlock();

        std::cout << "end of second thread\n";
    });

    first_client.join();
    second_client.join();
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}