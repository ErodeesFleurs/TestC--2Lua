set(LUA_LIB_NAME lua)

set(LUA_LIB_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")

add_library(${LUA_LIB_NAME} STATIC 
lua/lapi.c
lua/lauxlib.c
lua/lbaselib.c
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
lua/ltests.c
lua/ltm.c
lua/lua.c
lua/lundump.c
lua/lutf8lib.c
lua/lvm.c
lua/lzio.c
)

# # 设置编译器标志（你可以根据需求修改）
# target_compile_definitions(${LUA_LIB_NAME} PRIVATE LUA_CORE)

# 输出静态库到指定目录
set_target_properties(${LUA_LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LUA_LIB_OUTPUT_DIR})

# 设定 Lua 头文件的搜索路径
target_include_directories(${LUA_LIB_NAME} PUBLIC ${LUA_SRC_DIR})
