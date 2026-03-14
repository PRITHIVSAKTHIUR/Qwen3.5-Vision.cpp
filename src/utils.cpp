#include "utils.hpp"
#include <iostream>

namespace utils {

void print_banner() {
    std::cout << "===========================================\n";
    std::cout << "        Qwen3.5-Vision C++ Web App         \n";
    std::cout << "===========================================\n";
}

void print_help(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --model_name <name>       HuggingFace Model Name (default: prithivMLmods/Qwen3.5-0.8B-Unredacted-MAX)\n";
    std::cout << "  --arch <architecture>     Model Architecture Class Name (default: Qwen3_5ForConditionalGeneration)\n";
    std::cout << "  --port <port>             Web App Port (default: 7860)\n";
    std::cout << "  --help                    Show this help message\n\n";
    std::cout << "Example:\n";
    std::cout << "  " << prog_name << " --model_name prithivMLmods/Qwen3.5-0.8B-Unredacted-MAX --arch Qwen3_5ForConditionalGeneration --port 8080\n";
}

}