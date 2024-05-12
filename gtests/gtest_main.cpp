#include <gtest/gtest.h>
#include <gmock/gmock.h>
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

//rename to testing consistensy
struct databaseTesting : ::testing::Test {
    static const database::connector connector;

    static const std::vector<std::string_view> tables;

    static constexpr uint32_t tablesCNT = 17;  //should be 17 if unhandeled tables are added to the database
    static constexpr uint32_t usersCOLUMNS = 3;
    static constexpr uint32_t passwordsCOLUMNS = 3;
    static constexpr uint32_t tokensCOLUMNS = 2;
    static constexpr uint32_t formsCOLUMNS = 6;
    static constexpr uint32_t viewsCOLUMNS = 4;
    static constexpr uint32_t filmsCOLUMNS = 4;
    static constexpr uint32_t producersCOLUMNS = 3;  //i think will be 2 in future
    static constexpr uint32_t producers_by_filmCOLUMNS = 2;
    static constexpr uint32_t invitesCOLUMNS = 5;
    static constexpr uint32_t actorsCOLUMNS = 2;
    static constexpr uint32_t actors_to_formCOLUMNS = 2;
    static constexpr uint32_t actors_to_filmCOLUMNS = 2;
    static constexpr uint32_t messagesCOLUMNS = 5;
    static constexpr uint32_t chatsCOLUMNS = 6;
    static constexpr uint32_t chat_membershipCOLUMNS = 3;
};

const database::connector databaseTesting::connector {};

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
struct ProtosAndPlaceholdersTesting : ::testing::Test {

    static constexpr uint32_t actor_preview_proto_fields = 2;
    static constexpr uint32_t actor_id = 21; 
    static constexpr std::string_view actor_full_name = "FirstName LastName ThirdName";

    static constexpr uint32_t user_preview_proto_fields = 2;
    static constexpr uint32_t user_id = 21;
    static constexpr std::string_view user_login = "Login";

    static constexpr uint32_t film_preview_proto_fields = 2;
    static constexpr uint32_t film_id = 21;
    static constexpr std::string_view film_title = "Title";

    static constexpr uint32_t film_full_page_proto_fields = 3;
    static constexpr std::string_view film_description = "Description";
    static const std::vector<genre> genres;

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

const std::vector<genre> ProtosAndPlaceholdersTesting::genres = {genre::ACTION, genre::COMEDY, genre::DRAMA};
const std::vector<genre> ProtosAndPlaceholdersTesting::favourite_genres = {genre::SCI_FI, genre::HISTORY, genre::DOCUMENTARY};

actor_preview ProtosAndPlaceholdersTesting::actor_preview_placeholder { ProtosAndPlaceholdersTesting::actor_id, std::string(ProtosAndPlaceholdersTesting::actor_full_name) };
user_preview ProtosAndPlaceholdersTesting::user_preview_placeholder { ProtosAndPlaceholdersTesting::user_id, std::string(ProtosAndPlaceholdersTesting::user_login) };
film_preview ProtosAndPlaceholdersTesting::film_preview_placeholder { ProtosAndPlaceholdersTesting::film_id, std::string(ProtosAndPlaceholdersTesting::film_title) };
film_full_page ProtosAndPlaceholdersTesting::film_full_page_placeholder { ProtosAndPlaceholdersTesting::film_preview_placeholder, std::string(ProtosAndPlaceholdersTesting::film_description), ProtosAndPlaceholdersTesting::genres };
film_on_user_page ProtosAndPlaceholdersTesting::film_on_user_page_placeholder { ProtosAndPlaceholdersTesting::film_full_page_placeholder, ProtosAndPlaceholdersTesting::film_ratio };


const std::vector<actor_preview> ProtosAndPlaceholdersTesting::favourite_actors = {ProtosAndPlaceholdersTesting::actor_preview_placeholder};
const std::vector<film_on_user_page> ProtosAndPlaceholdersTesting::watched_films = {ProtosAndPlaceholdersTesting::film_on_user_page_placeholder};

user_full_page ProtosAndPlaceholdersTesting::user_full_page_placeholder { ProtosAndPlaceholdersTesting::user_preview_placeholder, std::string(ProtosAndPlaceholdersTesting::quoete), std::string(ProtosAndPlaceholdersTesting::about), ProtosAndPlaceholdersTesting::favourite_genres, 
ProtosAndPlaceholdersTesting::favourite_actors, ProtosAndPlaceholdersTesting::watched_films };

message ProtosAndPlaceholdersTesting::message_placeholder { std::string(ProtosAndPlaceholdersTesting::message_text), ProtosAndPlaceholdersTesting::user_preview_placeholder, std::string(ProtosAndPlaceholdersTesting::time_stamp) };

const std::vector<user_preview> ProtosAndPlaceholdersTesting::chat_members = {ProtosAndPlaceholdersTesting::user_preview_placeholder};

chat_preview ProtosAndPlaceholdersTesting::chat_preview_placeholder { ProtosAndPlaceholdersTesting::chat_id, ProtosAndPlaceholdersTesting::is_group_chat, std::string(ProtosAndPlaceholdersTesting::chat_name), std::string(ProtosAndPlaceholdersTesting::chat_description), std::string(ProtosAndPlaceholdersTesting::chat_time_stamp),
 ProtosAndPlaceholdersTesting::message_placeholder, ProtosAndPlaceholdersTesting::chat_members };

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
}

void check_actor_preview(const common::ActorPreview &proto, const actor_preview &placeholder) {
    EXPECT_EQ(proto.actor_id(), placeholder.id);
    EXPECT_EQ(proto.full_name(), placeholder.full_name);
}

void check_film_preview(const common::FilmPreview &proto, const film_preview &placeholder) {
    EXPECT_EQ(proto.film_id(), placeholder.id);
    EXPECT_EQ(proto.title(), placeholder.title);
}

void check_film_full_page(const common::FilmFullPage &proto, const film_full_page &placeholder) {
    check_film_preview(proto.preview(), placeholder.preview);
    EXPECT_EQ(proto.description(), placeholder.description);
    for (int i = 0; i < proto.genres_size(); i++) {
        EXPECT_EQ(proto.genres(i), static_cast<common::Genre>(placeholder.genres[i]));
    }
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

// TEST(Functional, TestImageReading) {
//     auto path = std::filesystem::current_path();
//     std::cerr << "path: " << path << "\n";
//     std::ifstream image_file;
//     try {
//         image_file.open("../gtests/test_in.jpg", std::ios_base::binary | std::ios_base::in);
//     } catch(std::exception &e) {
//         std::cerr << "Failed to open file: " << e.what() << '\n';
//         std::exit(1);
//     }
//     if (image_file.is_open()) {
//         std::cerr << "file opened\n";
//     } else {
//         std::cerr << "file not opened\n";
//         std::exit(1);
//     }
//     std::vector<char> buffer((std::istreambuf_iterator<char>(image_file)), std::istreambuf_iterator<char>());    
//     image_file.close();
//     std::cerr << "read: " << buffer.size() << " bytes\n";
//     std::ofstream out("test_out.jpg", std::ios_base::binary | std::ios_base::out);
//     out.write(reinterpret_cast<char*>(buffer.data()), static_cast<int>(buffer.size()));
//     out.close();
//     // std::exit(0);
//     // std::vector<unsigned char> image;
// }


TEST_F(databaseTesting, TestConnection) {
    ASSERT_NE(connector.con, nullptr);
    ASSERT_TRUE(connector.con->is_open()) << "Connection to database failed"; 
}

// TEST_F(databaseTesting, PutAndGetPhoto) {
//     static const std::string name = "name";
//     std::ifstream image_file;
//     image_file.open("../gtests/test_in.jpg", std::ios_base::binary | std::ios_base::in);
//     std::vector<char> buffer((std::istreambuf_iterator<char>(image_file)), std::istreambuf_iterator<char>());
//     std::cerr << "writted size: " << buffer.size() << " bytes\n";
//     image_file.close();
//     static constexpr std::string_view put_query = "INSERT INTO photo_testing (film_name, photo_bytes) VALUES ($1, $2)";
//     pqxx::binarystring bs(buffer.data(), buffer.size());
//     std::cerr << "size of binarystring: " << bs.size() << std::endl;
//     std::basic_string<std::byte> binary_std;
//     for (const auto& byte : buffer) {
//         binary_std.push_back(static_cast<std::byte>(byte));
//     }
//     std::cerr << "size of binary string std: " << binary_std.size() << std::endl;
//     // const auto result_first = connector.txn->exec_params(pqxx::zview(put_query), name, bs);
//     const auto result_first = connector.txn->exec_params(pqxx::zview(put_query), name, binary_std);
//     static constexpr std::string_view get_query = "SELECT photo_bytes FROM photo_testing WHERE film_name = $1";
//     const auto result_second = connector.txn->exec_params(pqxx::zview(get_query), name);
//     for (const auto& row : result_second) {
//         // auto bytes_std = row[0].as<std::byte*>();
//         auto bytes_pqxx = row[0].as<pqxx::binarystring>();
//         auto bytes = row[0].as<std::string>();
//         std::cerr << "read in bs: " << bytes_pqxx.size() << " bytes\n";
//         std::cerr << "read in string: " << bytes.size() << " bytes\n";
//         // std::cerr << "read in std::byte: " << bytes_std << " bytes\n";
//         // std::cerr << bytes << std::endl;
//         std::ofstream out("mem.jpg", std::ios_base::binary | std::ios_base::out);
//         out.write(bytes_pqxx.get(), bytes_pqxx.size());
//         // out.write(reinterpret_cast<char*>(bytes.data()), static_cast<int>(bytes.size()));
//         out.close();
//     }
//     // connector.txn->commit();
//     // std::exit(0);
// }

TEST_F(databaseTesting, TestPhotoCreationFromBytea) {
    static const std::string film_name = "John Carter";
    static constexpr std::string_view quer = "SELECT cover FROM films WHERE title = $1";
    const auto result = connector.txn->exec_params(pqxx::zview(quer), film_name);
    for (const auto& row : result) {
        // auto bytes = row[0].as<char*>();
        // auto bytes = row[0].as<std::string>();
        auto bytes = row[0].as<std::basic_string<std::byte>>(); 
        
        std::ofstream out(film_name + ".jpg", std::ios_base::binary | std::ios_base::out);
        std::cerr << "size: " << sizeof(std::byte) * bytes.size() << std::endl;
        out.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
        out.close();
        std::exit(0);
    }
}

TEST_F(databaseTesting, TestTimestamps) {
    static constexpr std::string_view quer = "SELECT CURRENT_TIMESTAMP";
    auto result = connector.txn->exec(quer);
    for (const auto& row : result) {
        auto var = row[0].as<std::string>();
        std::cerr << "current time in psql format: " << var << std::endl;
    // Input timestamp string
    std::string timestampStr = "2024-05-07 18:34:23.640258+03";

    // Parse the timestamp string into a time_point
    std::istringstream iss(timestampStr);
    std::tm tm = {};
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    // Convert time_point to string without microseconds
    auto timeInSeconds = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    std::time_t time = std::chrono::system_clock::to_time_t(timeInSeconds);
    std::tm truncatedTm = *std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&truncatedTm, "%Y-%m-%d %H:%M:%S");
    std::string timestampWithoutMicroseconds = oss.str();

    // Output the parsed timestamp without microseconds
    std::cout << "Timestamp without microseconds: " << timestampWithoutMicroseconds << std::endl;

    }

}

TEST_F(databaseTesting, TestMakingDefault) {
    static constexpr std::string_view table_creating = "CREATE TABLE test (id SERIAL PRIMARY KEY, name VARCHAR(50) NOT NULL, time_created TIMESTAMP DEFAULT NOW())";
    connector.txn->exec(table_creating);
    static constexpr std::string_view insert = "INSERT INTO test (name) VALUES ('test') RETURNING to_char(time_created, 'YYYY-MM-DD HH24:MI:SS')";
    const auto result = connector.txn->exec(insert);
    for (const auto& row : result) {
        auto var = row[0].as<std::string>();
        std::cerr << "time_created: " << var << std::endl;
    }
    static constexpr std::string_view delete_table = "DROP TABLE test";
    connector.txn->exec(delete_table);
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

// TEST(chat_service, TestUnsavedStreamClosed) {
//     // std::mutex m;
//     // std::condition_variable cv;
//     // bool ready = false;

//     std::thread ([&]() {
//         auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
//         auto stub = chat_service::ChatHandler::NewStub(channel);
//         auto context = new grpc::ClientContext();
//         auto stream = stub->HandleChat(context);
//         chat_service::ChatRequests request;

//         std::thread ([&]() {
//             chat_service::ChatResponses response;
//             while (stream->Read(&response)) {
//                 if (response.has_accept_message()) {
//                     std::cout << "Message accepted" << std::endl;
//                 } else {
//                     std::cout << "get status: " << response.response_with_status().status() << std::endl;
//                 }
//             }
//             std::cout << "reader finished" << std::endl;
//         }).detach();

//         request.mutable_init()->set_user_id(1);
//         stream->Write(request);
//         std::cerr << "init request written\n";
//         stream->WritesDone();
//         std::cerr << "writes done\n";
//         context->TryCancel();
//         std::cerr << "context canceled\n";
//         // stream->Finish();
//         // std::cerr << "stream finished\n";
//     }).join();
// }

TEST(chat_service, TestChatService) {
    std::mutex m;
    std::condition_variable cv;
    std::condition_variable cv_ready_for_second;
    bool ready = false, ready_for_second = false;

    constexpr int first_user_id = 93, second_user_id = 99, chat_id = 1;

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

        request.mutable_init()->set_user_id(first_user_id);
        stream->Write(request);

        std::unique_lock<std::mutex> l(m);
        ready = true;
        cv.notify_all();
        l.unlock();
        std::cout << "second client notified\n";

        chat_service::ChatRequests request2;
        request2.mutable_send_message()->mutable_msg()->set_sender_id(first_user_id);
        request2.mutable_send_message()->mutable_msg()->set_chat_id(chat_id);
        request2.mutable_send_message()->mutable_msg()->set_message_text("Hello");
        stream->Write(request2);

        std::unique_lock<std::mutex> l1(m);
        cv_ready_for_second.wait(l1, [&](){ return ready_for_second; });
        l1.unlock();

        chat_service::ChatRequests request3;
        request2.mutable_send_message()->mutable_msg()->set_sender_id(first_user_id);
        request2.mutable_send_message()->mutable_msg()->set_chat_id(chat_id);
        request2.mutable_send_message()->mutable_msg()->set_message_text("Word");
        stream->Write(request2);
        std::cout << "wrote second msg\n";

        std::this_thread::sleep_for(std::chrono::seconds(2));

        stream->WritesDone();
        stream->Finish();
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
                    EXPECT_EQ(response.accept_message().msg().sender().user_id(), first_user_id);
                    EXPECT_EQ(response.accept_message().msg().chat_id(), chat_id);
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

        request.mutable_init()->set_user_id(second_user_id);
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
        stream->Finish();
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

    // ::testing::GTEST_FLAG(filter) = "MyTestCase.*";
    return RUN_ALL_TESTS();
}