#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace ascii85 {

/**
 * Кодирует данные в ASCII85
 * @param data входные данные для кодирования
 * @return закодированная строка в ASCII85
 */
std::string encode(const std::vector<uint8_t>& data);

/**
 * Декодирует данные из ASCII85
 * @param encoded закодированная строка в ASCII85
 * @return декодированные данные
 * @throws std::invalid_argument если входные данные некорректны
 */
std::vector<uint8_t> decode(const std::string& encoded);

/**
 * Кодирует блок из 4 байт в 5 символов ASCII85
 * @param data блок данных (до 4 байт)
 * @param original_bytes количество исходных байт в блоке
 * @return закодированный блок
 */
std::string encode_block(const std::vector<uint8_t>& data, size_t original_bytes);

/**
 * Декодирует блок из 5 символов ASCII85 в 4 байта
 * @param block закодированный блок
 * @return декодированные байты
 * @throws std::invalid_argument если блок некорректен
 */
std::vector<uint8_t> decode_block(const std::string& block);

} // namespace ascii85
