#include "Drawable.hpp"
#include "Process.hpp"
#include "lua.hpp"

struct testT {
  int a;
  int b;
  auto getA() -> int { return a; }
  auto getB() -> int { return b; }
};

static int test_getA(lua_State* L) {
  testT* t = (testT*)luaL_checkudata(L, 1, "testT");  // 获取 userdata 对象
  lua_pushinteger(L, t->getA());
  return 1;  // 返回一个结果
}

static int test_getB(lua_State* L) {
  testT* t = (testT*)luaL_checkudata(L, 1, "testT");  // 获取 userdata 对象
  lua_pushinteger(L, t->getB());
  return 1;  // 返回一个结果
}

extern "C" int init(lua_State* L) {
  lua_newtable(L);

  lua_pushcfunction(L, [](lua_State* L) -> int {
    lua_pushinteger(L, Process::getPid());
    return 1;
  });
  lua_setfield(L, -2, "getPid");

  lua_pushcfunction(L, [](lua_State* L) -> int {
    lua_pushinteger(L, 42);
    return 1;
  });
  lua_setfield(L, -2, "getAnswer");

  lua_pushcfunction(L, [](lua_State* L) -> int {
    int a = luaL_checkinteger(L, 1);
    int b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, a + b);
    return 1;
  });
  lua_setfield(L, -2, "add");

  lua_pushcfunction(L, [](lua_State* L) -> int {
    // 创建 test 对象
    testT* t = (testT*)lua_newuserdata(L, sizeof(testT));
    t->a = luaL_checkinteger(L, 1);  // 设置 a
    t->b = luaL_checkinteger(L, 2);  // 设置 b

    // 为 test 类型创建元表
    luaL_newmetatable(L, "testT");

    // 将 test 的成员函数绑定到元表
    lua_pushcfunction(L, test_getA);
    lua_setfield(L, -2, "getA");

    lua_pushcfunction(L, test_getB);
    lua_setfield(L, -2, "getB");

    // 设置元表的 __index 元方法，允许通过元表访问成员函数
    lua_pushvalue(L, -1);  // 把元表的引用推到栈上
    lua_setfield(L, -2, "__index");

    // 获取并设置元表
    lua_setmetatable(L, -2);  // 设置元表到 userdata 上

    return 1;
  });
  lua_setfield(L, -2, "newTest");

  return 1;
}