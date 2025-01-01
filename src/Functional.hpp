#pragma once

#include "lua.hpp"
namespace LuaFunctional {
int l_map(lua_State* L);

int l_range(lua_State* L);

int l_reduce(lua_State* L);

int l_chain(lua_State* L);

int l_copy(lua_State* L);

}  // namespace LuaFunctional
