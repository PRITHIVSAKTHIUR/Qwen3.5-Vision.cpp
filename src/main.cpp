#include <iostream>
#include <string>
#include <pybind11/embed.h>
#include "vision_app.hpp"
#include "utils.hpp"

namespace py = pybind11;

int main(int argc, char* argv[]) {
    utils::print_banner();

    std::string model_name = "prithivMLmods/Qwen3.5-0.8B-Unredacted-MAX";
    std::string architecture = "Qwen3_5ForConditionalGeneration";
    int port = 7860;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            utils::print_help(argv[0]);
            return 0;
        } else if (arg == "--model_name" && i + 1 < argc) {
            model_name = argv[++i];
        } else if (arg == "--arch" && i + 1 < argc) {
            architecture = argv[++i];
        } else if (arg == "--port" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        }
    }

    std::cout << "Model Name: " << model_name << "\n";
    std::cout << "Architecture: " << architecture << "\n";
    std::cout << "Port: " << port << "\n\n";

    try {
        // Initialize Python interpreter
        py::scoped_interpreter guard{}; 

        bool success = vision_app::launch_app(
            model_name, architecture, port
        );

        if (success) {
            std::cout << "\nWeb app closed successfully!\n";
            return 0;
        } else {
            std::cerr << "\nWeb app failed.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize embedded Python: " << e.what() << "\n";
        return 1;
    }
}