#include "ascii85.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

void print_usage(const char* program_name) {
    std::cerr << "Использование: " << program_name << " [опции]\n";
    std::cerr << "Опции:\n";
    std::cerr << "  -e,     Кодировать данные в ASCII85 (по умолчанию)\n";
    std::cerr << "  -d,     Декодировать данные из ASCII85\n";
    std::cerr << "  -s,     Постепенное чтение/запись (по умолчанию)\n";
    std::cerr << "  -b,     Чтение до конца, затем вывод\n";
    std::cerr << "  -h,     Показать эту справку\n";
    std::cerr << "\n";
    std::cerr << "Читает данные из STDIN и выводит результат в STDOUT\n";
    std::cerr << "При ошибке декодирования завершается с кодом ≠ 0\n";
}

int main(int argc, char* argv[]) {
    bool decode_mode = false;
    bool stream_mode = true;
    
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
    
        if (arg.size() >= 2 && arg[0] == '-') {
            for (size_t k = 1; k < arg.size(); ++k) {
                switch (arg[k]) {
                    case 'd': decode_mode = true;  break;
                    case 'e': decode_mode = false; break;
                    case 's': stream_mode = true;  break;
                    case 'b': stream_mode = false; break;
                    default:
                        std::cerr << "Неизвестная опция: -" << arg[k] << "\n";
                        print_usage(argv[0]);
                        return 1;
                }
            }
            continue;
        }
    
        std::cerr << "Неизвестная опция: " << arg << "\n";
        print_usage(argv[0]);
        return 1;
    }

    try {
        if (decode_mode) {
            
            if (stream_mode) {
                std::string current_line;
                char ch;
                while (std::cin.read(&ch, 1)) {
                    if (ch == '\n') {
                        std::string cleaned;
                        for (char c : current_line) {
                            if (!std::isspace(static_cast<unsigned char>(c))) cleaned.push_back(c);
                        }
                        if (!cleaned.empty()) {
                            std::vector<uint8_t> decoded = ascii85::decode(cleaned);
                            if (!decoded.empty()) {
                                std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
                                std::cout.flush();
                            }
                        }
                        current_line.clear();
                    } else {
                        current_line.push_back(ch);
                    }
                }
                if (!current_line.empty()) {
                    std::string cleaned;
                    for (char c : current_line) {
                        if (!std::isspace(static_cast<unsigned char>(c))) cleaned.push_back(c);
                    }
                    if (!cleaned.empty()) {
                        std::vector<uint8_t> decoded = ascii85::decode(cleaned);
                        if (!decoded.empty()) {
                            std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
                            std::cout.flush();
                        }
                    }
                }
            } else {
                std::string input;
                char buf[4096];
                while (std::cin.read(buf, sizeof(buf))) {
                    input.append(buf, static_cast<size_t>(std::cin.gcount()));
                }
                if (std::cin.gcount() > 0) {
                    input.append(buf, static_cast<size_t>(std::cin.gcount()));
                }
                std::vector<uint8_t> decoded = ascii85::decode(input);
                if (!decoded.empty()) {
                    std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
                }
            }
            
        } else {
            if (stream_mode) {
                std::vector<uint8_t> current_block;
                char ch;
                while (std::cin.read(&ch, 1)) {
                    if (ch == '\n') {
                        if (!current_block.empty()) {
                            std::string encoded = ascii85::encode(current_block);
                            if (!encoded.empty()) {
                                std::cout.write(encoded.data(), static_cast<std::streamsize>(encoded.size()));
                            }
                            const char nl = '\n';
                            std::cout.write(&nl, 1);
                            std::cout.flush();
                            current_block.clear();
                        } else {
                            const char nl = '\n';
                            std::cout.write(&nl, 1);
                            std::cout.flush();
                        }
                    } else {
                        current_block.push_back(static_cast<uint8_t>(static_cast<unsigned char>(ch)));
                    }
                }
                if (!current_block.empty()) {
                    std::string encoded = ascii85::encode(current_block);
                    if (!encoded.empty()) {
                        std::cout.write(encoded.data(), static_cast<std::streamsize>(encoded.size()));
                    }
                    std::cout.flush();
                }
            } else {
                std::vector<uint8_t> input;
                char buf[4096];
                while (std::cin.read(buf, sizeof(buf))) {
                    input.insert(input.end(), reinterpret_cast<unsigned char*>(buf), reinterpret_cast<unsigned char*>(buf) + std::cin.gcount());
                }
                if (std::cin.gcount() > 0) {
                    input.insert(input.end(), reinterpret_cast<unsigned char*>(buf), reinterpret_cast<unsigned char*>(buf) + std::cin.gcount());
                }
                std::string encoded = ascii85::encode(input);
                if (!encoded.empty()) {
                    std::cout.write(encoded.data(), static_cast<std::streamsize>(encoded.size()));
                }
                {
                    const char nl = '\n';
                    std::cout.write(&nl, 1);
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
