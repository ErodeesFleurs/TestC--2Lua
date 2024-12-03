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

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")
set(THIRD_PARTY_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")


# 添加头文件和源文件
include_directories(${LUA_INCLUDE_DIR})
# 编译 Lua 静态库
add_subdirectory(${THIRD_PARTY_DIR})

# 添加目标
add_library(test SHARED)


file(GLOB_RECURSE HEADER_FILES src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
target_sources(test PRIVATE ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(test ${THIRD_PARTY_LIB_DIR}/liblua.a)

# set_target_properties(test PROPERTIES
#     PREFIX ""
#     SUFFIX ".dll"
# )