add_requires("lua", "sol2", "boost")


-- linux
target("test")
    set_kind("shared")
    add_links("pthread")
    add_headerfiles("src/*.hpp")
    add_files("src/*.cpp")
    add_packages("lua", "sol2", "boost")
    set_languages("c++20")
    set_warnings("all")
    set_optimize("fastest")

