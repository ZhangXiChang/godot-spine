add_rules("mode.debug", "mode.release")

add_requires("godotcpp4")

target("godot-spine")
    set_kind("shared")
    add_files("./src/*.cpp")
    add_packages("godotcpp4")
