#include <vector>
#include <string>

//Q::maybe store it in proto will be easier and more efficient ???

namespace database {
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

    struct actor_preview {
        std::string first_name;
        std::string last_name;
    };


    //or full page (with more info) if we become crazy
    /*struct actor_with_photo : actor_preview {

    };*/

    //for search mostly
    struct film_previev {
        std::string title;
        // std::vector<unsigned char> cover;
    };

    struct film_full_page : film_previev {
        std::string description;
        std::vector<genre> genres;
    };

    struct user_preview {
        //std::vector<unsigned char> photo;
        std::string nickname;
    };

    struct user_full_page {
        std::string quote;
        std::string about;
        std::vector<genre> fauvorite_genres;
        std::vector<actor_preview> fauvorite_actors;
        std::vector<film_previev> watched_films;
    };

    struct message {
        std::string message_text;
        user_preview sender;
        std::string time_stamp;
    };

    struct chat_preview {
        std::string name;
        message last_message;
        std::vector<unsigned char> cover;
    };

    struct user_chats {
        std::vector<chat_preview> user_chats;
    };

    struct last_messages {
        std::vector<message> messages;
    };

    struct invite {
        user_preview sender;
        std::string invite_text;
        invite_status status;
    };

    struct invites_recivied {
        std::vector<invite> invites;
    };

    struct invites_sent {
        std::vector<invite> invites;
    };
}