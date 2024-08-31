add_rules("mode.debug", "mode.release")

add_requires("godotcpp4","spine-runtimes")

target("godot-spine")
    set_kind("shared")
    set_languages("c17","cxx17")
    add_files("./src/*.cpp")
    add_includedirs("./include")
    add_packages("godotcpp4","spine-runtimes")

    on_install(function (target)
        os.cp(target:targetfile(),path.join(os.projectdir(),"addons",target:name(),target:targetfile():split("build")[1]))
    end)
