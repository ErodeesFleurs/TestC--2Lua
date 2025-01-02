cmake_minimum_required(VERSION 3.14)
project(test)

# set(CMAKE_SYSTEM_NAME Windows)
# set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
# set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
# set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译警告和优化选项
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
else()
    add_compile_options(-Wall -Wextra)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/${CMAKE_BUILD_TYPE})

# 设置thirdparty路径
set(LUA_SRC_DIR "${CMAKE_SOURCE_DIR}/thirdparty/lua")
set(LUA_INCLUDE_DIR "${LUA_SRC_DIR}")
set(STB_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/stb_image")

# set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")

# add_subdirectory(${THIRD_PARTY_DIR})
# include_directories(${LUA_INCLUDE_DIR})

# 添加 STB_IMAGE 头文件
file(GLOB STB_IMAGE_HEADERS
    "${STB_IMAGE_INCLUDE_DIR}/*.h"
)

# 添加 Lua 源文件
file(GLOB LUA_SOURCES
    "${LUA_SRC_DIR}/*.c"
)

# 添加 Lua 头文件
file(GLOB LUA_HEADERS
    "${LUA_SRC_DIR}/*.h"
)

# 构建 Lua 静态库
add_library(lua_static STATIC ${LUA_SOURCES})
target_include_directories(lua_static PUBLIC ${LUA_INCLUDE_DIR})
set_target_properties(lua_static PROPERTIES OUTPUT_NAME "lua")

# 为 Lua 静态库添加 -fPIC
if (UNIX)
    target_compile_options(lua_static PRIVATE -fPIC)
endif()

# 添加主目标
add_library(test SHARED)

# 添加源文件
file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
target_sources(test PRIVATE ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(test PRIVATE ${STB_IMAGE_INCLUDE_DIR})

# 链接 Lua 静态库
target_link_libraries(test PRIVATE lua_static)
