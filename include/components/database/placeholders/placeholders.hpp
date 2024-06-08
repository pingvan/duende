#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <string_view>
#include <vector>
#include "common/common.pb.h"
#include "services/authservice/authservice.pb.h"
#include "services/chat_service/chat_service.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "services/profile_service/profile.pb.h"
#include "services/search_service/search_service.pb.h"

enum class genre {
    ACTION,
    ADVENTURE,
    COMEDY,
    DRAMA,
    HORROR,
    SCI_FI,
    FANTASY,
    THRILLER,
    MYSTERY,
    ROMANCE,
    ANIMATION,
    DOCUMENTARY,
    CRIME,
    WESTERN,
    MUSIC,
    HISTORY,
    WAR,
    BIOGRAPHICAL,
    SPORTS,
    FAMILY,
    FOREIGN,
    TV_MOVIE
};

enum class chat_events_types {
    NAME_CHANGED,
    DESCRIPTION_CHANGED,
    COVER_CHANGED,
    USER_ENTERED,
    USER_LEFT,
    USER_REMOVED
};

struct actor_preview {
    actor_preview() = default;

    actor_preview(const uint32_t &id_, const std::string &full_name_)
        : id(id_), full_name(full_name_) {
    }

    uint32_t id;
    std::string full_name;

    void convert_to_proto(common::ActorPreview *proto) const {
        proto->set_actor_id(id);
        proto->set_full_name(full_name);
    }
};

struct film_preview {
    film_preview() = default;

    film_preview(const uint32_t &id_, const std::string &title_)
        : id(id_), title(title_) {
    }

    film_preview(
        const uint32_t &id_,
        const std::string &title_,
        const std::string &preview_cover_url_
    )
        : id(id_), title(title_), preview_cover_url(preview_cover_url_) {
    }

    uint32_t id;
    std::string title;
    std::optional<std::string> preview_cover_url;

    void convert_to_proto(common::FilmPreview *proto) const {
        proto->set_film_id(id);
        proto->set_title(title);
        if (preview_cover_url.has_value()) {
            proto->set_preview_cover_url(preview_cover_url.value());
        }
    }
};

struct film_full_page {
    film_preview preview;
    std::string description;
    std::vector<genre> genres;
    std::optional<std::string> full_page_cover_url;

    void convert_to_proto(common::FilmFullPage *proto) const {
        auto prewiew_proto = new common::FilmPreview;
        preview.convert_to_proto(prewiew_proto);
        proto->set_allocated_preview(prewiew_proto);
        proto->set_description(description);
        for (const auto &genre : this->genres) {
            proto->add_genres(static_cast<common::Genre>(genre));
        }
        if (full_page_cover_url.has_value()) {
            proto->set_full_page_cover_url(full_page_cover_url.value());
        }
    }
};

struct film_on_user_page {
    film_preview preview;
    uint32_t ratio;

    void convert_to_proto(common::FilmOnUserPage *proto) const {
        auto *film_preview_proto = new common::FilmPreview;
        preview.convert_to_proto(film_preview_proto);
        proto->set_allocated_preview(film_preview_proto);
        proto->set_ratio(ratio);
    }
};

struct user_preview {
    user_preview() = default;

    user_preview(const int &id_, const std::string &login_)
        : id(id_), login(login_) {
    }

    user_preview(
        const int &id_,
        const std::string &login_,
        const std::basic_string<std::byte> &photo_
    )
        : id(id_), login(login_), photo(photo_) {
    }

    uint32_t id;
    std::string login;
    std::optional<std::basic_string<std::byte>> photo;

    void convert_to_proto(common::UserPreview *response) const {
        response->set_user_id(id);
        response->set_login(login);
        if (photo.has_value()) {
            std::string string_photo;
            std::transform(
                photo.value().begin(), photo.value().end(),
                std::back_inserter(string_photo),
                [](std::byte byte_) { return static_cast<char>(byte_); }
            );
            response->set_photo(string_photo);
        }
    }
};

struct user_full_page {
    user_preview prewiew;
    std::string quote;
    std::string about;
    std::vector<genre> fauvorite_genres;
    std::vector<actor_preview> fauvorite_actors;
    std::vector<film_on_user_page> watched_films;

    void convert_to_proto(common::UserFullPage *proto) const {
        auto *protobuf_prewiew = new common::UserPreview;
        prewiew.convert_to_proto(protobuf_prewiew);
        proto->set_allocated_preview(protobuf_prewiew);
        proto->set_quote(quote);
        proto->set_about(about);
        for (const auto &genre : fauvorite_genres) {
            proto->add_favorite_genres(static_cast<common::Genre>(genre));
        }
        for (const auto &actor : fauvorite_actors) {
            auto actor_proto = proto->add_favorite_actors();
            actor.convert_to_proto(actor_proto);
        }
        for (const auto &film : this->watched_films) {
            auto film_proto = proto->add_watched_films();
            film.convert_to_proto(film_proto);
        }
    }
};

struct message {
    std::string message_text;
    user_preview sender;
    std::string time_stamp;

    void convert_to_proto(chat_service::MessageReceived *proto) const {
        proto->set_message_text(message_text);
        proto->set_time_stamp(time_stamp);
        auto *sender_proto = new common::UserPreview;
        sender.convert_to_proto(sender_proto);
        proto->set_allocated_sender(sender_proto);
    }
};

struct chat_preview {
    uint32_t chat_id;
    bool is_group_chat;
    std::string name;
    std::string description;
    std::string time_stamp;
    message last_message;
    std::vector<user_preview> chat_members;
    std::optional<std::basic_string<std::byte>> chat_cover;

    void convert_to_proto(chat_service::ChatPreview *proto) const {
        proto->set_chat_id(chat_id);
        proto->set_name(name);
        proto->set_description(description);
        proto->set_time_stamp(time_stamp);
        auto *last_message_proto = new chat_service::MessageReceived;
        last_message.convert_to_proto(last_message_proto);
        proto->set_allocated_last_message(last_message_proto);
        for (const auto &user_preview_ : chat_members) {
            auto user_preview_proto = proto->add_chat_members();
            user_preview_.convert_to_proto(user_preview_proto);
        }
        proto->set_is_group_chat(is_group_chat);
        if (chat_cover.has_value()) {
            std::string string_chat_cover;
            std::transform(
                chat_cover.value().begin(), chat_cover.value().end(),
                std::back_inserter(string_chat_cover),
                [](std::byte byte_) { return static_cast<char>(byte_); }
            );
            proto->set_chat_cover(string_chat_cover);
        }
    }
};

struct chat_event {
    chat_events_types event;
    std::optional<user_preview> user;
    std::string time_stamp;
    std::optional<user_preview> user_affected;

    void convert_to_proto(chat_service::ChatEvent *proto) const {
        proto->set_event_type(static_cast<chat_service::EventType>(event));
        if (user.has_value()) {
            auto user_proto = new common::UserPreview;
            user->convert_to_proto(user_proto);
            proto->set_allocated_user(user_proto);
        }
        proto->set_time_stamp(time_stamp);
        if (user_affected.has_value()) {
            auto user_affected_proto = new common::UserPreview;
            user_affected->convert_to_proto(user_affected_proto);
            proto->set_allocated_user_affected(user_affected_proto);
        }
    }
};

struct chat_events {
    std::vector<chat_event> events;

    void convert_to_proto(chat_service::ChatEvents *proto) const {
        for (const auto &event : events) {
            auto event_proto = proto->add_events();
            event.convert_to_proto(event_proto);
        }
    }
};

struct user_chats {
    std::vector<chat_preview> user_chats;

    void convert_to_proto(chat_service::UserChats *proto) const {
        for (const auto &chat : user_chats) {
            auto *proto_chat = proto->add_user_chats();
            chat.convert_to_proto(proto_chat);
        }
    }
};

struct last_messages {
    std::vector<message> messages;

    void convert_to_proto(chat_service::LastMessages *proto) const {
        for (const auto &message : messages) {
            auto *proto_message = proto->add_messages();
            message.convert_to_proto(proto_message);
        }
    }
};

struct invite {
    struct group_chat_preview {
        uint32_t chat_id;
        std::string name;
        std::string description;
        std::vector<user_preview> chat_members;
        std::optional<std::basic_string<std::byte>> chat_cover;

        void convert_to_proto(common::GroupChatPreview *proto) const {
            proto->set_chat_id(chat_id);
            proto->set_name(name);
            proto->set_description(description);
            for (const auto &user_preview_ : chat_members) {
                auto user_preview_proto = proto->add_members();
                user_preview_.convert_to_proto(user_preview_proto);
            }
            if (chat_cover.has_value()) {
                std::string string_chat_cover;
                std::transform(
                    chat_cover.value().begin(), chat_cover.value().end(),
                    std::back_inserter(string_chat_cover),
                    [](std::byte byte_) { return static_cast<char>(byte_); }
                );
                proto->set_chat_cover(string_chat_cover);
            }
        }
    };

    uint32_t invite_id;
    user_preview sender;
    std::string invite_text;
    std::string time_stamp;
    std::optional<group_chat_preview> chat_preview;

    void convert_to_proto(invitation::Invite *proto) const {
        proto->set_invite_id(invite_id);
        proto->set_invite_text(invite_text);
        auto *prewiew_proto = new common::UserPreview;
        sender.convert_to_proto(prewiew_proto);
        proto->set_allocated_sender(prewiew_proto);
        proto->set_time_stamp(time_stamp);
        if (chat_preview.has_value()) {
            auto *chat_preview_proto = new common::GroupChatPreview;
            chat_preview->convert_to_proto(chat_preview_proto);
            proto->set_allocated_chat_preview(chat_preview_proto);
        }
    }
};

struct invites_recivie {
    std::vector<invite> invites;

    void convert_to_proto(invitation::InvitesReceived *proto) const {
        for (const auto &invite : invites) {
            auto *proto_invite = proto->add_invites();
            invite.convert_to_proto(proto_invite);
        }
    }
};

struct invites_sent {
    std::vector<invite> invites;

    void convert_to_proto(invitation::InvitesSent *proto) const {
        for (const auto &invite : invites) {
            auto *proto_invite = proto->add_invites();
            invite.convert_to_proto(proto_invite);
        }
    }
};
