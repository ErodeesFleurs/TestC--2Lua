#include "lua.hpp"
#include "sol/sol.hpp"

#include "Net.hpp"
#include "Promise.hpp"
#include "Process.hpp"

Promise::Promise<std::optional<std::string>> fileRevicer(std::string_view ip, std::string_view port, std::string_view path) {
    Promise::Promise<std::optional<std::string>> promise(std::async(std::launch::async, Net::fileRevicer, ip, port, path));
    return promise;
}

Promise::Promise<std::optional<std::string>> fileSender(std::string_view ip, std::string_view port, std::string_view path) {
    Promise::Promise<std::optional<std::string>> promise(std::async(std::launch::async, Net::fileSender, ip, port, path));
    return promise;
}

class Point {
public:
    Point(int x, int y) : x(x), y(y) {}
    Point& operator=(const Point& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    Point operator+(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point operator-(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    int x;
    int y;
};

extern "C" int init(lua_State* L) {
    sol::state_view lua(L);
    sol::table module = lua.create_table();
    module.set_function("getPid", &Process::getPid);
    module.set_function("fileRevicer", &fileRevicer);
    module.set_function("fileSender", &fileSender);
    module.new_usertype<Point>("Point",
        sol::constructors<Point(int, int)>(),
        "x", &Point::x,
        "y", &Point::y,
        "add", &Point::operator+,
        "sub", &Point::operator-);
    module.push();
    return 1;
    // return sol::stack::call_lua(L, 1, open_my_object);
}