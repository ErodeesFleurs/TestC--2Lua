set(LUA_LIB_NAME lua)

set(LUA_LIB_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")

set(EXT_LIBS_SOURCES
lua/lapi.c
lua/lauxlib.c
lua/lbaselib.c
lua/lbitlib.c
lua/lcode.c
lua/lcorolib.c
lua/lctype.c
lua/ldblib.c
lua/ldebug.c
lua/ldo.c
lua/ldump.c
lua/lfunc.c
lua/lgc.c
lua/linit.c
lua/liolib.c
lua/llex.c
lua/lmathlib.c
lua/lmem.c
lua/loadlib.c
lua/lobject.c
lua/lopcodes.c
lua/loslib.c
lua/lparser.c
lua/lstate.c
lua/lstring.c
lua/lstrlib.c
lua/ltable.c
lua/ltablib.c
lua/ltm.c
lua/lundump.c
lua/lutf8lib.c
lua/lvm.c
lua/lzio.c
)

set(EXT_LIBS_HEADERS
lua/lauxlib.h
lua/lua.h
lua/lua.hpp
lua/luaconf.h
lua/lualib.h
)

add_library(ext_libs OBJECT ${EXT_LIBS_SOURCES} ${EXT_LIBS_HEADERS})