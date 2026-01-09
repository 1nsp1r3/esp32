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

// Structure compacte pour le dictionnaire LZW
struct LZWEntry {
    uint16_t prefix;  // Code précédent
    uint8_t suffix;   // Caractère ajouté
};

/**
 * Reconstruit une séquence depuis le dictionnaire de manière itérative
 * Écrit de la fin vers le début pour éviter la récursion
 */
static size_t reconstructSequence(const LZWEntry* dict, uint16_t code, uint8_t* buffer, size_t pos) {
    // Calcule d'abord la longueur en remontant la chaîne
    uint16_t temp_code = code;
    size_t length = 0;

    while (temp_code >= LZW_INITIAL_CODE_SIZE) {
        length++;
        temp_code = dict[temp_code - LZW_INITIAL_CODE_SIZE].prefix;
    }
    length++; // +1 pour le caractère initial (< 256)

    // Écrit les caractères en ordre inverse (de la fin vers le début)
    size_t write_pos = pos + length - 1;
    temp_code = code;

    while (temp_code >= LZW_INITIAL_CODE_SIZE) {
        buffer[write_pos--] = dict[temp_code - LZW_INITIAL_CODE_SIZE].suffix;
        temp_code = dict[temp_code - LZW_INITIAL_CODE_SIZE].prefix;
    }
    buffer[write_pos] = static_cast<uint8_t>(temp_code);

    return pos + length;
}

/**
 * Obtient le premier caractère d'une séquence
 */
static uint8_t getFirstChar(const LZWEntry* dict, uint16_t code) {
    while (code >= LZW_INITIAL_CODE_SIZE) {
        code = dict[code - LZW_INITIAL_CODE_SIZE].prefix;
    }
    return static_cast<uint8_t>(code);
}

/**
 *
 */
uint8_t* i3Unzip(const uint8_t* data, size_t size, size_t expected_size) {
    if (!data || size == 0) {
        return nullptr;
    }

    uint8_t* output = new uint8_t[expected_size];
    size_t output_pos = 0;

    // Dictionnaire compact : seulement prefix + suffix
    LZWEntry* dictionary = new LZWEntry[LZW_MAX_CODE - LZW_INITIAL_CODE_SIZE + 1];
    uint16_t next_code = LZW_INITIAL_CODE_SIZE;

    // Buffer temporaire pour reconstruction de séquences
    uint8_t* temp_buffer = new uint8_t[LZW_MAX_CODE];

    // Decode et traite les codes à la volée
    size_t byte_pos = 0;
    uint16_t old_code = 0;
    bool first_code = true;

    while (byte_pos < size) {
        // Décode le premier code de 12 bits
        if (byte_pos + 1 >= size) break;

        uint16_t code = (static_cast<uint16_t>(data[byte_pos]) << 4) |
                       ((data[byte_pos + 1] >> 4) & 0x0F);

        // Traite le code
        size_t sequence_length;

        if (first_code) {
            // Premier code : doit être < 256
            if (code >= LZW_INITIAL_CODE_SIZE) {
                delete[] output;
                delete[] dictionary;
                delete[] temp_buffer;
                return nullptr;
            }
            output[output_pos++] = static_cast<uint8_t>(code);
            old_code = code;
            first_code = false;
        } else {
            // Reconstruit la séquence
            if (code < LZW_INITIAL_CODE_SIZE) {
                temp_buffer[0] = static_cast<uint8_t>(code);
                sequence_length = 1;
            } else if (code < next_code) {
                sequence_length = reconstructSequence(dictionary, code, temp_buffer, 0);
            } else if (code == next_code) {
                // Cas spécial : code pas encore dans le dictionnaire
                sequence_length = reconstructSequence(dictionary, old_code, temp_buffer, 0);
                temp_buffer[sequence_length] = temp_buffer[0];
                sequence_length++;
            } else {
                delete[] output;
                delete[] dictionary;
                delete[] temp_buffer;
                return nullptr;
            }

            // Copie la séquence dans le résultat
            std::memcpy(output + output_pos, temp_buffer, sequence_length);
            output_pos += sequence_length;

            // Ajoute au dictionnaire
            if (next_code <= LZW_MAX_CODE) {
                dictionary[next_code - LZW_INITIAL_CODE_SIZE].prefix = old_code;
                dictionary[next_code - LZW_INITIAL_CODE_SIZE].suffix = temp_buffer[0];
                next_code++;
            }

            old_code = code;
        }

        // Décode le second code de 12 bits si disponible
        if (byte_pos + 2 < size) {
            code = ((static_cast<uint16_t>(data[byte_pos + 1]) & 0x0F) << 8) |
                   data[byte_pos + 2];

            // Reconstruit la séquence
            if (code < LZW_INITIAL_CODE_SIZE) {
                temp_buffer[0] = static_cast<uint8_t>(code);
                sequence_length = 1;
            } else if (code < next_code) {
                sequence_length = reconstructSequence(dictionary, code, temp_buffer, 0);
            } else if (code == next_code) {
                sequence_length = reconstructSequence(dictionary, old_code, temp_buffer, 0);
                temp_buffer[sequence_length] = temp_buffer[0];
                sequence_length++;
            } else {
                delete[] output;
                delete[] dictionary;
                delete[] temp_buffer;
                return nullptr;
            }

            // Copie la séquence dans le résultat
            std::memcpy(output + output_pos, temp_buffer, sequence_length);
            output_pos += sequence_length;

            // Ajoute au dictionnaire
            if (next_code <= LZW_MAX_CODE) {
                dictionary[next_code - LZW_INITIAL_CODE_SIZE].prefix = old_code;
                dictionary[next_code - LZW_INITIAL_CODE_SIZE].suffix = temp_buffer[0];
                next_code++;
            }

            old_code = code;
            byte_pos += 3;
        } else {
            byte_pos += 2;
        }
    }

    delete[] dictionary;
    delete[] temp_buffer;
    return output;
}
