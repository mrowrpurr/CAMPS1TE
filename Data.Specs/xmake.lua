add_requires("vcpkg::snowhouse", "fmt")

-- Include the Specs.cpp library for tests
local specs_path = os.getenv("SPECS_CPP")
if specs_path then
    includes(specs_path)
else
    print("SPECS_CPP not set")
    return
end

target("Camps1te.Data.Specs")
    set_kind("binary")
    add_packages("vcpkg::snowhouse")
    add_deps("Camps1te.Data", "Specs")
    add_files("*.cpp")
    add_includedirs(".")
    add_defines("STRING_FORMAT_USE_FMT")

    -- duplicated from Data :(
    add_packages("nlohmann_json", "_Log_", "spdlog", "string_format", "fmt", {public = true})