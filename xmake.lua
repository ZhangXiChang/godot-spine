add_repositories("repo ./repo")

add_rules("mode.debug", "mode.release")

add_requires("godotcpp4","spine-cpp")

target("godot-spine")
    set_kind("shared")
    set_languages("c17","cxx17")
    add_files("./src/*.cpp")
    add_includedirs("./include")
    add_packages("godotcpp4","spine-cpp")

    on_install(function (target)
        os.cp(target:targetfile(),path.join(os.projectdir(),"lib",target:targetfile():split("build")[1]))
    end)
