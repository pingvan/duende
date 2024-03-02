#include "hash.hpp"
#include <openssl/sha.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

std::string generate_hash(const std::string &password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(
        reinterpret_cast<const unsigned char *>(password.data()),
        password.length(), hash
    );
    std::stringstream ss;
    for (const unsigned char i : hash) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(i);
    }
    return ss.str();
}

std::string generate_salt(int size) {
    static const std::string symbols = {
        "0123456789!@#$%^&*"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(
        5, static_cast<int>(symbols.size())
    );
    std::stringstream random_symbols;
    for (int i = 0; i < size; i++) {
        random_symbols << symbols[distrib(gen)];
    }
    return random_symbols.str();
}
