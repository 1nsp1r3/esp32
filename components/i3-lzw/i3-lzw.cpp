#include "i3-lzw.h"

/**
 *
 */
uint8_t* i3Zip(const uint8_t* data, size_t size, size_t& out_size) {
    if (!data || size == 0) {
        out_size = 0;
        return nullptr;
    }

    std::unordered_map<std::string, uint16_t> dictionary;

    // Initialize dictionary with single-byte sequences
    for (uint16_t i = 0; i < LZW_INITIAL_CODE_SIZE; i++) {
        dictionary[std::string(1, static_cast<char>(i))] = i;
    }

    uint16_t next_code = LZW_INITIAL_CODE_SIZE;
    std::string current;
    std::vector<uint16_t> compressed;

    for (size_t i = 0; i < size; i++) {
        char c = static_cast<char>(data[i]);
        std::string combined = current + c;

        if (dictionary.find(combined) != dictionary.end()) {
            current = combined;
        } else {
            compressed.push_back(dictionary[current]);

            if (next_code <= LZW_MAX_CODE) {
                dictionary[combined] = next_code++;
            }

            current = std::string(1, c);
        }
    }

    if (!current.empty()) {
        compressed.push_back(dictionary[current]);
    }

    // Convert 12-bit codes to byte array
    // Each 2 codes (24 bits) = 3 bytes
    size_t num_codes = compressed.size();
    size_t byte_size = (num_codes * 3 + 1) / 2;  // ceil(num_codes * 1.5)

    uint8_t* output = new uint8_t[byte_size];
    size_t byte_pos = 0;

    for (size_t i = 0; i < num_codes; i += 2) {
        uint16_t code1 = compressed[i];

        if (i + 1 < num_codes) {
            uint16_t code2 = compressed[i + 1];
            // Pack two 12-bit codes into 3 bytes
            output[byte_pos++] = (code1 >> 4) & 0xFF;
            output[byte_pos++] = ((code1 & 0x0F) << 4) | ((code2 >> 8) & 0x0F);
            output[byte_pos++] = code2 & 0xFF;
        } else {
            // Last code (odd number of codes)
            output[byte_pos++] = (code1 >> 4) & 0xFF;
            output[byte_pos++] = (code1 & 0x0F) << 4;
        }
    }

    out_size = byte_size;
    return output;
}

/**
 *
 */
uint8_t* i3Unzip(const uint8_t* data, size_t size, size_t& out_size) {
    if (!data || size == 0) {
        out_size = 0;
        return nullptr;
    }

    // Decode byte array to 12-bit codes
    std::vector<uint16_t> codes;
    size_t byte_pos = 0;

    while (byte_pos < size) {
        if (byte_pos + 1 < size) {
            uint16_t code1 = (static_cast<uint16_t>(data[byte_pos]) << 4) |
                           ((data[byte_pos + 1] >> 4) & 0x0F);
            codes.push_back(code1);

            if (byte_pos + 2 < size) {
                uint16_t code2 = ((static_cast<uint16_t>(data[byte_pos + 1]) & 0x0F) << 8) |
                               data[byte_pos + 2];
                codes.push_back(code2);
                byte_pos += 3;
            } else {
                byte_pos += 2;
            }
        } else {
            break;
        }
    }

    if (codes.empty()) {
        out_size = 0;
        return nullptr;
    }

    // Build reverse dictionary
    std::unordered_map<uint16_t, std::string> dictionary;
    for (uint16_t i = 0; i < LZW_INITIAL_CODE_SIZE; i++) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    uint16_t next_code = LZW_INITIAL_CODE_SIZE;
    std::string result;

    uint16_t old_code = codes[0];
    result += dictionary[old_code];

    for (size_t i = 1; i < codes.size(); i++) {
        uint16_t code = codes[i];
        std::string entry;

        if (dictionary.find(code) != dictionary.end()) {
            entry = dictionary[code];
        } else if (code == next_code) {
            entry = dictionary[old_code] + dictionary[old_code][0];
        } else {
            out_size = 0;
            return nullptr;  // Invalid compressed data
        }

        result += entry;

        if (next_code <= LZW_MAX_CODE) {
            dictionary[next_code++] = dictionary[old_code] + entry[0];
        }

        old_code = code;
    }

    out_size = result.size();
    uint8_t* output = new uint8_t[out_size];
    std::memcpy(output, result.c_str(), out_size);

    return output;
}
