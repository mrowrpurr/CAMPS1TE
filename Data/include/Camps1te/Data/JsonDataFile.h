#pragma once

#include <_Log_.h>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "DataFile.h"
#include "JsonKeys.h"

namespace Camps1te::Data {

    class JsonDataFile : public DataFile {
        std::filesystem::path _path;

        std::vector<std::string> GetKeys(nlohmann::json jsonObject) {
            std::vector<std::string> keys;
            for (auto& [key, value] : jsonObject.items()) keys.push_back(key);
            return keys;
        }

        nlohmann::json LoadJson() {
            std::ifstream file(_path.string());
            if (!file.is_open()) {
                _Log_("Failed to open file {}", _path.string());
                return {};
            }
            nlohmann::json json;
            file >> json;
            return json;
        }

    public:
        JsonDataFile(const std::filesystem::path& path) : _path(path) {}

        std::vector<std::string> GetPluginNames() override {
            auto json = LoadJson();
            if (json.contains(JsonKeys::TOPLEVEL_PLUGINS))
                return GetKeys(json[JsonKeys::TOPLEVEL_PLUGINS]);
            else return {};
        }

        std::vector<std::string> GetDataTypes(const std::string& pluginName) override {
            auto json = LoadJson();
            if (json.contains(JsonKeys::TOPLEVEL_PLUGINS)) {
                auto plugins = json[JsonKeys::TOPLEVEL_PLUGINS];
                if (plugins.contains(pluginName)) return GetKeys(plugins[pluginName]);
            }
            return {};
        }
    };
}
