target("Camps1te.Editor")
    set_kind("binary")
    add_files("*.cpp", "../Resources/Editor.qrc")
    add_includedirs("include")
    add_headerfiles("include/(**).h")
    add_qt()
    add_packages("nlohmann_json", "_Log_", "spdlog", "string_format")
    add_deps("Camps1te.Data", "Camps1te.UI", "Camps1te.MapView", "Camps1te.Game")
