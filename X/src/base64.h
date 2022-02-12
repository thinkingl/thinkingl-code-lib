#pragma once

#include <string>

std::string base64Encode(const unsigned char *src, size_t len);

std::string base64Decode(const void* data, const size_t len);