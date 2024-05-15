#include "Net.hpp"

#include "boost/asio.hpp"

#include <fstream>

namespace Net {
    using namespace boost::asio;
    using ip::tcp;

    //接收文件
    std::optional<std::string> fileRevicer(std::string_view ip, std::string_view port, std::string_view path) {
        try {
            io_context io_context;
            tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(std::string(port))));
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::ofstream file(std::string(path), std::ios::binary);
            if (!file) {
                return "Failed to open file";
            }

            char data[1024];
            size_t len;
            while ((len = socket.read_some(buffer(data))) > 0) {
                file.write(data, len);
            }

            return std::nullopt;
        } catch (std::exception& e) {
            return e.what();
        }        
    }

    //发送文件
    std::optional<std::string> fileSender(std::string_view ip, std::string_view port, std::string_view path) {
        try {
            io_context io_context;
            tcp::resolver resolver(io_context);
            tcp::resolver::query query{std::string(ip), std::string(port)};
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(ip, port);
            tcp::socket socket(io_context);
            connect(socket, endpoint_iterator);

            std::ifstream file(std::string(path), std::ios::binary);
            if (!file) {
                return "Failed to open file";
            }

            char data[1024];
            while (file.read(data, sizeof(data)).gcount() > 0) {
                write(socket, buffer(data, file.gcount()));
            }

            return std::nullopt;
        } catch (std::exception& e) {
            return e.what();
        }
    }

    //获取本地IP地址
    std::optional<std::string> getLocalIPAddress() {
        try {
            boost::asio::io_context io_context;
            boost::asio::ip::tcp::resolver resolver(io_context);
            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(boost::asio::ip::tcp::v4(), boost::asio::ip::host_name(), "");
            
            for (const auto& endpoint : endpoints) {
                boost::asio::ip::address addr = endpoint.endpoint().address();
                if (addr.is_v4()) {
                    return addr.to_string();
                }
            }
            return {};
        } catch (std::exception& e) {
            return e.what();
        }
    }
}