#pragma once

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>

namespace aes {

void handleErrors();

std::vector<unsigned char> encrypt(const std::vector<unsigned char>& plaintext,
                                const std::vector<unsigned char>& key,
                                const std::vector<unsigned char>& iv);

std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext,
                                const std::vector<unsigned char>& key,
                                const std::vector<unsigned char>& iv);

std::string bytes_to_hex(const std::vector<unsigned char>& bytes);

std::vector<unsigned char> hex_to_bytes(const std::string& hex) ;
    
struct encryptor {

    std::string encrypt(const std::string &message_text);
    std::string decrypt(const std::string &message_text);

};

}
