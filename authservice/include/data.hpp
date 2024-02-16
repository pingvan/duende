#ifndef DATA_HPP_
#define DATA_HPP_

#include <vector>
#include "jwt.hpp"

struct User {
private:
    std::string username;
    std::string email;
    std::string password;
    // more information coming soon...
public:
    User() = default;
    User(std::string username, std::string email, std::string password) : username(username), email(email), password(password) {}
    User(std::string username, std::string password) : username(username), password(password) {}
    
    std::string GetUsername() { return username; }
    std::string GetEmail() { return email; }
    std::string GetPassword() { return password; }
    void SetUsername(std::string name) { this->username = username; }
    void SetEmail(std::string email) { this->email = email; }
    void SetPassword(std::string password) { this->password = password; }

    bool operator==(const User &user) {
        return (this->username == user.username || this->email == user.email) && this->password == user.password;
    }
};

inline std::vector <User> users;

std::string jwt_token(std::string username);

#endif
