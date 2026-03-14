#pragma once
#include <string>

namespace vision_app {
    bool launch_app(
        const std::string& model_name,
        const std::string& architecture,
        int port
    );
}