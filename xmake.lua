add_requires("lua v5.3.6", "sol2", "boost")


-- linux
target("test")
    set_kind("shared")
    add_headerfiles("src/*.hpp")
    add_files("src/*.cpp")
    add_packages("lua v5.3.6", "sol2", "boost")
    set_languages("c++20")
    set_warnings("all")
    set_optimize("fastest")
    set_targetdir("$(projectdir)/dist/$(mode)")

