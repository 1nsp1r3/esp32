#ifndef I3_LZW_H
#define I3_LZW_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <string>

static const uint16_t          LZW_MAX_CODE = 4095;  //12-bit codes
static const uint16_t LZW_INITIAL_CODE_SIZE = 256;

uint8_t* i3Zip(const uint8_t* data, size_t size, size_t& out_size);
uint8_t* i3Unzip(const uint8_t* data, size_t size, size_t& out_size);

#endif
