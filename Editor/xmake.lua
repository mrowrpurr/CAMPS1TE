add_requires("nlohmann_json", "fmt", "_Log_", "spdlog")

target("Camps1te Editor")
    set_kind("binary")
    add_files("*.cpp", "../Resources/Editor.qrc")
    add_includedirs("include")
    add_qt()
    add_packages("nlohmann_json", "fmt", "_Log_", "spdlog")
