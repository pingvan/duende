#pragma once

#include <vector>
#include <string>
#include <string_view>

#include "services/authservice/authservice.pb.h"
#include "services/chat_service/chat_service.pb.h"
// #include "services/profile_service/profile_service.pb.h"
// #include "services/invitation/invitation.pb.h"
#include "services/invitation_service/invitation.pb.h"
#include "services/search_service/search_service.pb.h"

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
        actor_preview() = default;
        actor_preview(const int &id_, const std::string &full_name_) : id(id_), full_name(full_name_) {}
        int id;
        std::string full_name;

        void convert_to_proto(authservice::ActorPreview *proto) const {
            proto->set_full_name(full_name);
        }

        void convert_to_proto(search_service::Actor *proto) const {
            proto->set_actor_id(id);
            proto->set_full_name(full_name);
        }
    };


    //or full page (with more info) if we become crazy
    /*struct actor_with_photo : actor_preview {

    };*/

    //for search mostly
    struct film_preview  {
        film_preview() = default;
        film_preview(const int &id_, const std::string &title_) : id(id_), title(title_) {}
        int id;
        std::string title;
        // std::vector<unsigned char> cover;

        void convert_to_proto(authservice::FilmPreview *proto) const {
            proto->set_title(title);
        }

        void convert_to_proto(search_service::Film *proto) const {
            proto->set_film_id(id);
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

    struct user_preview  {
        // std::vector<unsigned char> photo;
        user_preview() = default;
        user_preview(const int &id_, const std::string &login_) : id(id_), login(login_) {}
        int id;
        std::string login;

        void convert_to_proto(authservice::UserPreview *response) const  {
            response->set_login(login);
        }

        void convert_to_proto(search_service::User *response) const  {
            response->set_user_id(id);
            response->set_login(login);
        }
    };

    struct user_full_page  {
        user_preview prewiew;
        std::string quote;
        std::string about;
        std::vector<genre> fauvorite_genres;
        std::vector<actor_preview> fauvorite_actors;
        std::vector<film_on_user_page> watched_films;

        void convert_to_proto(authservice::UserFullPage *proto) const {
            try {
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
            } catch (std::exception &e) {
                std::cout << e.what() << '\n';
            }
        }
    };

    struct message  {
        std::string message_text;
        user_preview sender;
        std::string time_stamp;

        void convert_to_proto(chat_service::MessageData *proto) const {
            proto->set_message_text(message_text);
            proto->set_time_stamp(time_stamp);
            auto *sender_proto = new authservice::UserPreview;
            sender.convert_to_proto(sender_proto);
            proto->set_allocated_sender(sender_proto);
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
            proto->set_chat_id(chat_id);
            proto->set_name(name);
            proto->set_description(description);
            proto->set_time_stamp(time_stamp);
            auto *last_message_proto = new chat_service::MessageData;
            last_message.convert_to_proto(last_message_proto);
            proto->set_allocated_last_message(last_message_proto);
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
        invite_status status;

        void convert_to_proto(invitation::Invite *proto) const {
            proto->set_invite_id(invite_id);
            proto->set_invite_text(invite_text);
            auto *prewiew_proto = new authservice::UserPreview;
            sender.convert_to_proto(prewiew_proto);
            proto->set_allocated_sender(prewiew_proto);
            proto->set_status(static_cast<invitation::InviteStatus>(status));
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
