#pragma once

#include <string>

std::string hash(const std::string &password);
std::string salt(int size);
