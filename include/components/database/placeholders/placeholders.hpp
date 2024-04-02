#pragma once

#include <vector>
#include <string>
#include <string_view>

#include "services/authservice/authservice.pb.h"
#include "services/chat_service/chat_service.pb.h"
// #include "services/profile_service/profile_service.pb.h"
#include "services/invite_service/invite_service.pb.h"

    enum class genre {
    action,
    adventure,
    comedy,
    drama,
    horror,
    sci_fi,
    fantasy,
    thriller,
    mystery,
    romance,
    animation,
    documentary,
    crime,
    western,
    music,
    history,
    war,
    biographical,
    sports,
    family,
    foreign,
    tv_movie
};

    enum class invite_status {sent, accepted, declined};

    struct actor_preview  {
        // std::string first_name;
        // std::string last_name;
        std::string full_name;

        void convert_to_proto(authservice::ActorPreview *proto) const {
            proto->set_full_name(full_name);
        }
    };


    //or full page (with more info) if we become crazy
    /*struct actor_with_photo : actor_preview {

    };*/

    //for search mostly
    struct film_preview  {
        std::string title;
        // std::vector<unsigned char> cover;

        void convert_to_proto(authservice::FilmPreview *proto) const {
            proto->set_title(title);
        }
    };

    struct film_full_page  {
        film_preview preview;
        std::string description;
        std::vector<genre> genres;

        void convert_to_proto(authservice::FilmFullPage *proto) const {
            auto prewiew_proto = new authservice::FilmPreview;
            preview.convert_to_proto(prewiew_proto);
            proto->set_allocated_preview(prewiew_proto);
            proto->set_description(description);
            for (const auto& genre : this->genres) {
                proto->add_genres(static_cast<authservice::Genre>(genre));
            }
        }
    };

    struct film_on_user_page  {
        film_full_page full_page;
        int ratio;

        void convert_to_proto(authservice::FilmOnUserPage *proto)  const {
            auto *film_full_page_proto = new authservice::FilmFullPage;
            full_page.convert_to_proto(film_full_page_proto);
            proto->set_allocated_full_page(film_full_page_proto);
            proto->set_ratio(ratio);
        }
    };

    struct user_preview  {//create
        // std::vector<unsigned char> photo;
        std::string login;

        void convert_to_proto(authservice::UserPreview *response) const  {
            response->set_login(login);
        }
    };

    struct user_full_page  {//create
        user_preview prewiew;
        std::string quote;
        std::string about;
        std::vector<genre> fauvorite_genres;
        std::vector<actor_preview> fauvorite_actors;
        std::vector<film_on_user_page> watched_films;

        void convert_to_proto(authservice::UserFullPage *proto) const {
            auto *protobuf_prewiew = new authservice::UserPreview;
            protobuf_prewiew->set_login(prewiew.login);
            proto->set_allocated_preview(protobuf_prewiew);
            proto->set_quote(quote);
            proto->set_about(about);
            for (const auto& genre : fauvorite_genres) {
                proto->add_favorite_genres(static_cast<authservice::Genre>(genre));
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

        void convert_to_proto(chat_service::Message *proto) const  {

        }
    };

    struct chat_preview  { //do as user_chat inner struct
        uint chat_id;
        std::string name;
        std::string description;
        std::string time_stamp;
        message last_message;
        // std::vector<unsigned char> cover;

        void convert_to_proto(chat_service::ChatPreview *proto) const  {

        }
    };

    struct user_chats  {//create
        std::vector<chat_preview> user_chats;

        void convert_to_proto(chat_service::UserChats *proto) const  {

        }
    };

    struct last_messages  {//create
        std::vector<message> messages;

        void convert_to_proto(chat_service::LastMessages *proto) const  {
            
        }
    };

    struct invite  {
        uint invite_id;
        user_preview sender;
        std::string invite_text;
        invite_status status;

        void convert_to_proto(invite_service::Invite *proto) const {
            proto->set_invite_id(invite_id);
            proto->set_invite_text(invite_text);
            auto *prewiew_proto = new authservice::UserPreview;
            sender.convert_to_proto(prewiew_proto);
            proto->set_allocated_sender(prewiew_proto);
            proto->set_status(static_cast<invite_service::InviteStatus>(status));
        }
    };

    struct invites_recivie  {//create
        std::vector<invite> invites;

        void convert_to_proto(invite_service::InvitesReceive *proto) const {
            for (const auto &invite : invites) {
                auto *proto_invite = proto->add_invites();
                invite.convert_to_proto(proto_invite);
            }
            
        }
    };

    struct invites_sent  {//create
        std::vector<invite> invites;

        void convert_to_proto(invite_service::InvitesSent *proto) const  {
            for (const auto &invite : invites) {
                auto *proto_invite = proto->add_invites();
                invite.convert_to_proto(proto_invite);
            }
        }
    };
