#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Net {
    std::optional<std::string> fileRevicer(std::string_view ip, std::string_view port, std::string_view path);
    std::optional<std::string> fileSender(std::string_view ip, std::string_view port, std::string_view path);
    std::optional<std::string> getLocalIPAddress();
}