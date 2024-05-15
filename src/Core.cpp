#include <unistd.h>

#include "lua.hpp"
#include "sol/sol.hpp"

#include "NetFile.hpp"
#include "Promise.hpp"

int getPid() {
    return getpid();
}

Promise::Promise<std::optional<std::string>> fileRevicer(std::string_view ip, std::string_view port, std::string_view path) {
    Promise::Promise<std::optional<std::string>> promise(std::async(std::launch::async, Net::fileRevicer, ip, port, path));
    return promise;
}

Promise::Promise<std::optional<std::string>> fileSender(std::string_view ip, std::string_view port, std::string_view path) {
    Promise::Promise<std::optional<std::string>> promise(std::async(std::launch::async, Net::fileSender, ip, port, path));
    return promise;
}

sol::table open_my_object(sol::this_state L) {
    sol::state_view lua(L);
    sol::table module = lua.create_table();
    module.set_function("getPid", &getPid);
    module.set_function("fileRevicer", &fileRevicer);
    module.set_function("fileSender", &fileSender);
    lua.new_usertype<Promise::Promise<std::optional<std::string>>>("Promise", 
        "finished", &Promise::Promise<std::optional<std::string>>::finished, 
        "succeeded", &Promise::Promise<std::optional<std::string>>::succeeded, 
        "result", &Promise::Promise<std::optional<std::string>>::result, 
        "error", &Promise::Promise<std::optional<std::string>>::error);
    return module;
}

extern "C" int init(lua_State* L) {
    return sol::stack::call_lua(L, 1, open_my_object);
}
