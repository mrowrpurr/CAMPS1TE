add_rules("mode.debug", "mode.release")

set_languages("c++20")

add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")
add_requires("nlohmann_json", "_Log_", "spdlog", "string_format")

if is_host("windows") then
    set_toolchains("msvc")
elseif is_host("macosx") then
    set_toolchains("clang")
elseif is_host("linux") then
    -- TODO
end

includes("xmake/qt.lua")
includes("**/xmake.lua")
