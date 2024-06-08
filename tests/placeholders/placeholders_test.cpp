#include <gtest/gtest.h>

#include "components/database/placeholders/placeholders.hpp"

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
film_on_user_page ProtosAndPlaceholdersTesting::film_on_user_page_placeholder { ProtosAndPlaceholdersTesting::film_preview_placeholder, ProtosAndPlaceholdersTesting::film_ratio };


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
    std::transform(placeholder.photo.value().begin(), placeholder.photo.value().end(), std::back_inserter(placeholder_photo), [](std::byte byte) { return static_cast<char>(byte); });
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
    check_film_preview(proto.preview(), placeholder.preview);
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