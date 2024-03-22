#pragma once

#include <vector>
#include <string>
#include <string_view>


namespace database {
    struct placeholder {
        virtual void convert_to_proto() = 0;
    };

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
    musical,
    historical,
    war,
    biographical,
    sports,
    family
    };

    enum class invite_status {sent, accepted, declined};

    struct actor_preview : placeholder {
        std::string first_name;
        std::string last_name;
    };


    //or full page (with more info) if we become crazy
    /*struct actor_with_photo : actor_preview {

    };*/

    //for search mostly
    struct film_preview : placeholder {
        std::string title;
        std::vector<unsigned char> cover;

        void convert_to_proto() override {

        }
    };

    struct film_full_page : placeholder {
        film_preview preview;
        std::string description;
        std::vector<genre> genres;

        void convert_to_proto() override {

        }
    };

    struct user_preview : placeholder {
        std::vector<unsigned char> photo;
        std::string login;

        void convert_to_proto() override {

        }
    };

    struct user_full_page : placeholder {
        std::string quote;
        std::string about;
        std::vector<genre> fauvorite_genres;
        std::vector<actor_preview> fauvorite_actors;
        std::vector<film_preview> watched_films;

        void convert_to_proto() override {

        }
    };

    struct message : placeholder {
        std::string message_text;
        user_preview sender;
        std::string time_stamp;

        void convert_to_proto() override {

        }
    };

    struct chat_preview : placeholder { //do as user_chat inner struct
        std::string name;
        message last_message;
        std::vector<unsigned char> cover;

        void convert_to_proto() override {

        }
    };

    struct user_chats : placeholder {
        std::vector<chat_preview> user_chats;

        void convert_to_proto() override {

        }
    };

    struct last_messages : placeholder {
        std::vector<message> messages;

        void convert_to_proto() override {

        }
    };

    struct invite : placeholder {
        user_preview sender;
        std::string invite_text;
        invite_status status;

        void convert_to_proto() override {

        }
    };

    struct invites_recivie : placeholder {
        std::vector<invite> invites;

        void convert_to_proto() override {

        }
    };

    struct invites_sent : placeholder {
        std::vector<invite> invites;

        void convert_to_proto() override {

        }
    };
}