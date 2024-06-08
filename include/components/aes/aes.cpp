#include "aes.hpp"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <iostream>

namespace aes {

void handleErrors() {
    unsigned long errCode;
    while ((errCode = ERR_get_error())) {
        char *err = ERR_error_string(errCode, nullptr);
        std::cerr << "Error: " << err << std::endl;
    }
    throw std::runtime_error("An error occurred");
}

std::vector<unsigned char> encrypt(const std::vector<unsigned char>& plaintext,
                                   const std::vector<unsigned char>& key,
                                   const std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1)
        handleErrors();

    int len;
    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));

    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1)
        handleErrors();
    int ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
        handleErrors();
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext,
                                   const std::vector<unsigned char>& key,
                                   const std::vector<unsigned char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1)
        handleErrors();

    int len;
    std::vector<unsigned char> plaintext(ciphertext.size());

    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1)
        handleErrors();
    int plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
        handleErrors();
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

std::string bytes_to_hex(const std::vector<unsigned char>& bytes) {
    static const char* hex_digits = "0123456789ABCDEF";
    std::string hex;
    hex.reserve(bytes.size() * 2);
    for (unsigned char byte : bytes) {
        hex.push_back(hex_digits[byte >> 4]);
        hex.push_back(hex_digits[byte & 0x0F]);
    }
    return hex;
}

std::vector<unsigned char> hex_to_bytes(const std::string& hex) {
    std::vector<unsigned char> bytes;
    bytes.reserve(hex.size() / 2);
    for (std::size_t i = 0; i < hex.size(); i += 2) {
        unsigned char byte = (std::stoi(hex.substr(i, 1), nullptr, 16) << 4)
                           + std::stoi(hex.substr(i + 1, 1), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

std::string encryptor::encrypt(const std::string& message_text) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    const char* key_env = std::getenv("KEY");
    const char* iv_env = std::getenv("IV");

    if (key_env == nullptr || iv_env == nullptr) {
        std::cerr << "Environment variables KEY and IV are not set." << std::endl;
    }

    std::string key_str(key_env);
    std::string iv_str(iv_env);

    std::vector<unsigned char> key = hex_to_bytes(key_str);
    std::vector<unsigned char> iv = hex_to_bytes(iv_str);

    std::vector<unsigned char> plaintext(message_text.begin(), message_text.end());
    std::vector<unsigned char> ciphertext = aes::encrypt(plaintext, key, iv);
    std::string encrypted_string = bytes_to_hex(ciphertext);

    ERR_free_strings();
    return encrypted_string;
}

std::string encryptor::decrypt(const std::string& encrypted_string) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    const char* key_env = std::getenv("KEY");
    const char* iv_env = std::getenv("IV");

    if (key_env == nullptr || iv_env == nullptr) {
        std::cerr << "Environment variables KEY and IV are not set." << std::endl;
    }

    std::string key_str(key_env);
    std::string iv_str(iv_env);

    std::vector<unsigned char> key = hex_to_bytes(key_str);
    std::vector<unsigned char> iv = hex_to_bytes(iv_str);

    std::vector<unsigned char> ciphertext = hex_to_bytes(encrypted_string);
    std::vector<unsigned char> decryptedtext = aes::decrypt(ciphertext, key, iv);
    std::string decrypted_string(decryptedtext.begin(), decryptedtext.end());

    ERR_free_strings();
    return decrypted_string;
}

} // namespace aes
