#include "Functional.hpp"

#include <functional>
#include <vector>

// 实现map map(function， array)
int LuaFunctional::l_map(lua_State* L) {
  // 获取参数个数
  int argc = lua_gettop(L);
  // 参数个数不为2，报错
  if (argc != 2) {
    lua_pushstring(L, "map function takes 2 arguments");
    lua_error(L);
    return 0;
  }
  luaL_checktype(L, 1, LUA_TFUNCTION);  // 第一个参数是函数
  luaL_checktype(L, 2, LUA_TTABLE);     // 第二个参数是表

  // 获取表的长度
  size_t len = lua_rawlen(L, 2);

  // 创建新的表作为返回值
  lua_newtable(L);

  // 遍历原始表
  for (size_t i = 1; i <= len; ++i) {
    // 获取表中的元素
    lua_pushinteger(L, i);  // 将索引推入栈
    lua_gettable(L, 2);     // 获取表中该索引的值

    // 调用传入的函数
    lua_pushvalue(L, 1);  // 将函数推入栈
    lua_insert(L, -2);    // 将函数移到值之前
    lua_call(L, 1, 1);

    // 获取返回值，并将其插入到新表中
    lua_pushinteger(L, i);  // 将索引推入栈
    lua_insert(L, -2);      // 将返回值放到新表中的正确位置
    lua_settable(L, -3);    // 将新值设置到新表
  }

  return 1;  // 返回新表
}

// 实现range(int)
int LuaFunctional::l_range(lua_State* L) {
  // 获取参数个数
  int argc = lua_gettop(L);
  // 参数个数不为1，报错
  if (argc != 1) {
    lua_pushstring(L, "range function takes 1 argument");
    lua_error(L);
    return 0;
  }
  int n = luaL_checkinteger(L, 1);  // 获取参数
  // 创建新的表
  lua_newtable(L);
  // 将1到n的整数放入表中
  for (int i = 1; i <= n; i++) {
    lua_pushinteger(L, i);
    lua_rawseti(L, -2, i);
  }
  return 1;  // 返回新表
}

// 实现reduce(function, array, init)
int LuaFunctional::l_reduce(lua_State* L) {
  // 获取参数个数
  int argc = lua_gettop(L);
  // 参数个数不为2，报错
  if (argc != 3) {
    lua_pushstring(L, "reduce function takes 3 arguments");
    lua_error(L);
    return 0;
  }
  luaL_checktype(L, 1, LUA_TFUNCTION);  // 第一个参数是函数
  luaL_checktype(L, 2, LUA_TTABLE);     // 第二个参数是表
  luaL_checkany(L, 3);                  // 第三个参数是初始值

  // 获取初始值
  lua_pushvalue(L, 3);  // 将初始值推入栈

  // 获取表的长度
  size_t len = lua_rawlen(L, 2);

  // 遍历表的每个元素，并应用reduce的函数
  for (size_t i = 1; i <= len; ++i) {
    // 获取表中的元素
    lua_pushinteger(L, i);  // 将索引推入栈
    lua_gettable(L, 2);     // 获取表中该索引的值

    // 获取累积值
    lua_pushvalue(L, -2);  // 将累积值推入栈

    // 调用传入的函数
    lua_pushvalue(L, 1);  // 将函数推入栈
    lua_insert(L, -3);    // 将函数移到累积值和当前值之前
    lua_call(L, 2, 1);
  }

  // 返回最终的累积结果
  return 1;
}

// 实现chain(array, array) 用于连接两个数组
int LuaFunctional::l_chain(lua_State* L) {
  // 获取参数个数
  int argc = lua_gettop(L);
  // 参数个数不为2，报错
  if (argc != 2) {
    lua_pushstring(L, "chain function takes 2 arguments");
    lua_error(L);
    return 0;
  }
  luaL_checktype(L, 1, LUA_TTABLE);  // 第一个参数是表
  luaL_checktype(L, 2, LUA_TTABLE);  // 第二个参数是表

  // 获取第一个表的长度
  size_t len1 = lua_rawlen(L, 1);
  // 获取第二个表的长度
  size_t len2 = lua_rawlen(L, 2);

  // 创建新的表
  lua_newtable(L);

  // 将第一个表的元素放入新表
  for (size_t i = 1; i <= len1; ++i) {
    lua_pushinteger(L, i);  // 将索引推入栈
    lua_gettable(L, 1);     // 获取表中该索引的值
    lua_pushinteger(L, i);  // 将索引推入栈
    lua_insert(L, -2);      // 将值放到新表中的正确位置
    lua_settable(L, -3);    // 将新值设置到新表
  }

  // 将第二个表的元素放入新表
  for (size_t i = 1; i <= len2; ++i) {
    lua_pushinteger(L, i);         // 将索引推入栈
    lua_gettable(L, 2);            // 获取表中该索引的值
    lua_pushinteger(L, len1 + i);  // 将索引推入栈
    lua_insert(L, -2);             // 将值放到新表中的正确位置
    lua_settable(L, -3);           // 将新值设置到新表
  }

  return 1;  // 返回新表
}

// 实现copy(array)
int LuaFunctional::l_copy(lua_State* L) {
  // 获取参数个数
  int argc = lua_gettop(L);
  // 参数个数不为1，报错
  if (argc != 1) {
    lua_pushstring(L, "copy function takes 1 argument");
    lua_error(L);
    return 0;
  }
  luaL_checktype(L, 1, LUA_TTABLE);  // 第一个参数是表
  // 获取数组长度
  int len = luaL_len(L, 1);
  // 创建一个新的数组
  lua_newtable(L);
  // 遍历数组
  for (int i = 1; i <= len; i++) {
    // 获取数组中的元素
    lua_rawgeti(L, 1, i);
    // 将元素放入新数组
    lua_rawseti(L, -2, i);
  }
  // 返回新数组
  return 1;
}