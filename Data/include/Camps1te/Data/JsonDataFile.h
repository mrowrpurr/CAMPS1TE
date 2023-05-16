#pragma once

#include <_Log_.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include "DataFile.h"
#include "JsonDataEntry.h"

namespace Camps1te::Data {

    class JsonDataFile : public DataFile {
        std::filesystem::path _path;
        nlohmann::json        _json;

        std::vector<std::string> GetKeys(nlohmann::json jsonObject) {
            std::vector<std::string> keys;
            for (auto& [key, value] : jsonObject.items()) keys.push_back(key);
            return keys;
        }

        nlohmann::json& LoadJson() {
            if (!_json.is_null()) return _json;
            std::ifstream file(_path.string());
            if (!file.is_open()) {
                _Log_("Failed to open file {}", _path.string());
                return _json;
            }
            file >> _json;
            return _json;
        }

    public:
        JsonDataFile(const std::filesystem::path& path) : _path(path) {}

        std::vector<std::unique_ptr<DataEntry>> GetEntries() override {
            std::vector<std::unique_ptr<DataEntry>> entries;

            auto& fileData = LoadJson()["data"];
            for (auto& [key, value] : fileData.items())
                entries.emplace_back(std::move(std::make_unique<JsonDataEntry>(key, value)));

            return entries;
        }
    };
}
