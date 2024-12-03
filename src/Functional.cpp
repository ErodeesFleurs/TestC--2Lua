#include "Functional.hpp"

#include <functional>
#include <vector>

// Helper function to check if a Lua value is a function
bool is_function(lua_State* L, int index) {
  return lua_type(L, index) == LUA_TFUNCTION;
}

// Helper function to get a Lua table as a std::vector
std::vector<lua_Number> get_table(lua_State* L, int index) {
  std::vector<lua_Number> result;
  lua_pushnil(L);  // first key
  while (lua_next(L, index - 1) != 0) {
    if (lua_isnumber(L, -1)) {
      result.push_back(lua_tonumber(L, -1));
    }
    lua_pop(L, 1);  // remove value, keep key for next iteration
  }
  return result;
}

// map function
int l_map(lua_State* L) {
  if (!lua_istable(L, 1) || !is_function(L, 2)) {
    return luaL_error(L, "Expected table and function");
  }

  std::vector<lua_Number> input = get_table(L, 1);
  lua_newtable(L);  // create result table

  for (size_t i = 0; i < input.size(); ++i) {
    lua_pushvalue(L, 2);          // push function
    lua_pushnumber(L, input[i]);  // push argument
    lua_call(L, 1, 1);            // call function with 1 argument and 1 result
    lua_rawseti(L, -2, i + 1);    // set result in table
  }

  return 1;  // return result table
}

// filter function
int l_filter(lua_State* L) {
  if (!lua_istable(L, 1) || !is_function(L, 2)) {
    return luaL_error(L, "Expected table and function");
  }

  std::vector<lua_Number> input = get_table(L, 1);
  lua_newtable(L);  // create result table
  int result_index = 1;

  for (size_t i = 0; i < input.size(); ++i) {
    lua_pushvalue(L, 2);          // push function
    lua_pushnumber(L, input[i]);  // push argument
    lua_call(L, 1, 1);            // call function with 1 argument and 1 result
    if (lua_toboolean(L, -1)) {
      lua_pushnumber(L, input[i]);
      lua_rawseti(L, -3, result_index++);  // set result in table
    }
    lua_pop(L, 1);  // remove function result
  }

  return 1;  // return result table
}

// reduce function
int l_reduce(lua_State* L) {
  if (!lua_istable(L, 1) || !is_function(L, 2) || !lua_isnumber(L, 3)) {
    return luaL_error(L, "Expected table, function, and initial value");
  }

  std::vector<lua_Number> input = get_table(L, 1);
  lua_Number accumulator = lua_tonumber(L, 3);

  for (size_t i = 0; i < input.size(); ++i) {
    lua_pushvalue(L, 2);             // push function
    lua_pushnumber(L, accumulator);  // push accumulator
    lua_pushnumber(L, input[i]);     // push current value
    lua_call(L, 2, 1);  // call function with 2 arguments and 1 result
    accumulator = lua_tonumber(L, -1);
    lua_pop(L, 1);  // remove function result
  }

  lua_pushnumber(L, accumulator);
  return 1;  // return accumulator
}

// find function
int l_find(lua_State* L) {
  if (!lua_istable(L, 1) || !is_function(L, 2)) {
    return luaL_error(L, "Expected table and function");
  }

  std::vector<lua_Number> input = get_table(L, 1);

  for (size_t i = 0; i < input.size(); ++i) {
    lua_pushvalue(L, 2);          // push function
    lua_pushnumber(L, input[i]);  // push argument
    lua_call(L, 1, 1);            // call function with 1 argument and 1 result
    if (lua_toboolean(L, -1)) {
      lua_pushnumber(L, input[i]);
      return 1;  // return the found value
    }
    lua_pop(L, 1);  // remove function result
  }

  lua_pushnil(L);  // return nil if not found
  return 1;
}

// every function
int l_every(lua_State* L) {
  if (!lua_istable(L, 1) || !is_function(L, 2)) {
    return luaL_error(L, "Expected table and function");
  }

  std::vector<lua_Number> input = get_table(L, 1);

  for (size_t i = 0; i < input.size(); ++i) {
    lua_pushvalue(L, 2);          // push function
    lua_pushnumber(L, input[i]);  // push argument
    lua_call(L, 1, 1);            // call function with 1 argument and 1 result
    if (!lua_toboolean(L, -1)) {
      lua_pushboolean(L, 0);
      return 1;  // return false if any element does not satisfy the predicate
    }
    lua_pop(L, 1);  // remove function result
  }

  lua_pushboolean(L, 1);  // return true if all elements satisfy the predicate
  return 1;
}

// Register functions
// extern "C" int luaopen_functional(lua_State* L) {
//   luaL_Reg functions[] = {{"map", l_map},       {"filter", l_filter},
//                           {"reduce", l_reduce}, {"find", l_find},
//                           {"every", l_every},   {NULL, NULL}};

//   luaL_newlib(L, functions);
//   return 1;
// }