target("Camps1te.Data")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**).h")
    add_packages("nlohmann_json", "_Log_", "spdlog", "string_format", {public = true})
