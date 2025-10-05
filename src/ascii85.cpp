#include "ascii85.h"
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace ascii85 {

    std::string encode_block(const std::vector<uint8_t>& data, size_t original_bytes) {
        if (original_bytes == 0 || original_bytes > 4) {
            throw std::invalid_argument("Invalid block size");
        }
        
        uint32_t num = 0;
        for (size_t i = 0; i < 4; ++i) {
            num = (num << 8) | (i < original_bytes ? data[i] : 0);
        }
        if (num == 0 && original_bytes == 4) {
            return "z";
        }

        std::string encoded;
        for (int i = 0; i < 5; ++i) {
            encoded.push_back(static_cast<char>(num % 85 + 33));
            num /= 85;
        }
        std::reverse(encoded.begin(), encoded.end());
        size_t encoded_length = original_bytes + 1;
        return encoded.substr(0, encoded_length);
    }

    std::vector<uint8_t> decode_block(const std::string& block) {
        if (block.empty()) {
            throw std::invalid_argument("Empty block");
        }
        
        if (block == "z") {
            return {0, 0, 0, 0};
        }
        
        if (block.length() > 5) {
            throw std::invalid_argument("Block too long");
        }
        
        std::string padded_block = block;
        while (padded_block.length() < 5) {
            padded_block += 'u';
        }
        
        for (char c : padded_block) {
            if (c < 33 || c > 117) {
                throw std::invalid_argument("Invalid character in block");
            }
        }
        
        uint32_t num = 0;
        for (char c : padded_block) {
            num = num * 85 + (c - 33);
        }
        
        std::vector<uint8_t> result;
        for (int i = 3; i >= 0; --i) {
            result.push_back(static_cast<uint8_t>((num >> (i * 8)) & 0xFF));
        }
        
        size_t original_bytes = 4 - (5 - block.length());
        result.resize(original_bytes);
        
        return result;
    }

    std::string encode(const std::vector<uint8_t>& data) {
        std::string result;
        
        for (size_t i = 0; i < data.size(); i += 4) {
            std::vector<uint8_t> block;
            size_t block_size = std::min(static_cast<size_t>(4), data.size() - i);
            
            for (size_t j = 0; j < block_size; ++j) {
                block.push_back(data[i + j]);
            }
            
            result += encode_block(block, block_size);
        }
        
        return result;
    }

    std::vector<uint8_t> decode(const std::string& encoded) {
        std::vector<uint8_t> result;
        
        std::string clean_encoded;
        for (char c : encoded) {
            if (!std::isspace(c)) {
                clean_encoded.push_back(c);
            }
        }
        
        for (size_t i = 0; i < clean_encoded.length(); ) {
            if (clean_encoded[i] == 'z') {
                result.insert(result.end(), {0, 0, 0, 0});
                i++;
            } else {
                size_t block_end = i;
                while (block_end < clean_encoded.length() && 
                    clean_encoded[block_end] != 'z' && 
                    block_end - i < 5) {
                    block_end++;
                }
                
                std::string block = clean_encoded.substr(i, block_end - i);
                
                if (block.length() < 1) {
                    throw std::invalid_argument("Incomplete block");
                }
                
                std::vector<uint8_t> decoded_block = decode_block(block);
                result.insert(result.end(), decoded_block.begin(), decoded_block.end());
                i = block_end;
            }
        }
        
        return result;
    }

}
