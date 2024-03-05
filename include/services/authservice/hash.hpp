#pragma once

#include <string>

std::string generate_hash(const std::string &password);
std::string generate_salt(int size);
