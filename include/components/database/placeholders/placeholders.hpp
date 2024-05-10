#pragma once

#include <cassert>
#include <vector>
#include <string>
#include <string_view>

#include "services/authservice/authservice.pb.h"
#include "services/chat_service/chat_service.pb.h"
#include "services/profile_service/profile.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "services/search_service/search_service.pb.h"
#include "common/common.pb.h"

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

// enum class invite_status { SENT, ACCEPTED, DECLINED };

    struct actor_preview  {
        actor_preview() = default;
        actor_preview(const uint &id_, const std::string &full_name_) : id(id_), full_name(full_name_) {}

        uint id;
        std::string full_name;

        void convert_to_proto(common::ActorPreview *proto) const {
            proto->set_actor_id(id);
            proto->set_full_name(full_name);
        }
    };


    struct film_preview  {
        film_preview() = default;
        film_preview(const uint &id_, const std::string &title_) : id(id_), title(title_) {}

        uint id;
        std::string title;
        // std::vector<unsigned char> cover;

        void convert_to_proto(common::FilmPreview *proto) const {
            proto->set_film_id(id);
            proto->set_title(title);
        }
    };

    struct film_full_page  {
        film_preview preview;
        std::string description;
        std::vector<genre> genres;

        void convert_to_proto(common::FilmFullPage *proto) const {
            auto prewiew_proto = new common::FilmPreview;
            preview.convert_to_proto(prewiew_proto);
            proto->set_allocated_preview(prewiew_proto);
            proto->set_description(description);
            for (const auto& genre : this->genres) {
                proto->add_genres(static_cast<common::Genre>(genre));
            }
        }
    };

    struct film_on_user_page  {
        film_full_page full_page;
        uint ratio;

        void convert_to_proto(common::FilmOnUserPage *proto)  const {
            auto *film_full_page_proto = new common::FilmFullPage;
            full_page.convert_to_proto(film_full_page_proto);
            proto->set_allocated_full_page(film_full_page_proto);
            proto->set_ratio(ratio);
        }
    };

    struct user_preview  {
        user_preview() = default;
        user_preview(const int &id_, const std::string &login_) : id(id_), login(login_) {}
        
        uint id;
        std::string login;
        // std::vector<unsigned char> photo;

        void convert_to_proto(common::UserPreview *response) const  {
            response->set_user_id(id);
            response->set_login(login);
        }
    };

    struct user_full_page  {
        // user_full_page() = default;   
        /*user_full_page(const user_preview &prewiew_, const std::string &quote_,
         const std::string &about_, const std::vector<genre> &genres_,
          const std::vector<actor_preview> &actors_, const std::vector<film_on_user_page> &films) 
          : prewiew(prewiew_), quote(quote_), about(about_), fauvorite_genres(genres_),
          fauvorite_actors(actors_), watched_films(films) {}*/

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
                for (const auto& genre : fauvorite_genres) {
                    proto->add_favorite_genres(static_cast<common::Genre>(genre));
                }
                for (const auto& actor : fauvorite_actors) {
                    auto actor_proto = proto->add_favorite_actors();
                    actor.convert_to_proto(actor_proto);
                }
                for (const auto& film : this->watched_films) {
                    auto film_proto = proto->add_watched_films();
                    film.convert_to_proto(film_proto);
                }
        }
    };

    struct message  {
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

    struct chat_preview  { 
        uint chat_id;
        bool is_group_chat;
        std::string name;
        std::string description;
        std::string time_stamp;
        message last_message;
        std::vector<user_preview> chat_members;
        // std::vector<unsigned char> cover;

        void convert_to_proto(chat_service::ChatPreview *proto) const  {
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
        }
    };

    struct user_chats  {
        std::vector<chat_preview> user_chats;

        void convert_to_proto(chat_service::UserChats *proto) const  {
            for (const auto &chat : user_chats) {
                auto *proto_chat = proto->add_user_chats();
                chat.convert_to_proto(proto_chat);
            }
        }
    };

    struct last_messages  {
        std::vector<message> messages;

        void convert_to_proto(chat_service::LastMessages *proto) const  {
            for (const auto &message : messages) {
                auto *proto_message = proto->add_messages();
                message.convert_to_proto(proto_message);
            }
        }
    };

    struct invite  {
        uint invite_id;
        user_preview sender;
        std::string invite_text;

        void convert_to_proto(invitation::Invite *proto) const {
            proto->set_invite_id(invite_id);
            proto->set_invite_text(invite_text);
            auto *prewiew_proto = new common::UserPreview;
            sender.convert_to_proto(prewiew_proto);
            proto->set_allocated_sender(prewiew_proto);
        }
    };

    struct invites_recivie  {
        std::vector<invite> invites;

        void convert_to_proto(invitation::InvitesReceived *proto) const {
            for (const auto &invite : invites) {
                auto *proto_invite = proto->add_invites();
                invite.convert_to_proto(proto_invite);
            }
            
        }
    };

    struct invites_sent  {
        std::vector<invite> invites;

        void convert_to_proto(invitation::InvitesSent *proto) const  {
            for (const auto &invite : invites) {
                auto *proto_invite = proto->add_invites();
                invite.convert_to_proto(proto_invite);
            }
        }
    };
