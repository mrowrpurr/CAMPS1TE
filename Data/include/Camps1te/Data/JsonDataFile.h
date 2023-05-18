#pragma once

#include <_Log_.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <unordered_map>

#include "DataFile.h"
#include "JsonRecord.h"

namespace Camps1te::Data {

    class JsonDataFile : public DataFile {
        std::filesystem::path                                        _path;
        nlohmann::json                                               _json;
        std::unordered_map<std::string, std::unique_ptr<JsonRecord>> _records;

        std::vector<std::string> GetKeys(nlohmann::json jsonObject) {
            std::vector<std::string> keys;
            for (auto& [key, value] : jsonObject.items()) keys.push_back(key);
            return keys;
        }

        nlohmann::json& GetJsonDocument() {
            if (!_json.is_null()) return _json;
            std::ifstream file(_path.string());
            if (!file.is_open()) {
                _Log_("Failed to open file {}", _path.string());
                return _json;
            }
            file >> _json;
            return _json;
        }

        std::optional<std::reference_wrapper<nlohmann::json>> GetDataSection() {
            auto& json = GetJsonDocument();
            if (json.is_null()) return std::nullopt;
            if (!json.contains("data")) {
                _Log_("File {} does not contain a 'data' section", _path.string());
                return std::nullopt;
            }
            return std::ref(json["data"]);
        }

    public:
        JsonDataFile(const std::filesystem::path& path) : _path(path) {}

        Record* GetRecord(const char* identifier) override {
            auto found = _records.find(identifier);
            if (found != _records.end()) {
                return found->second.get();
            } else {
                auto dataSection = GetDataSection();
                if (!dataSection.has_value()) return nullptr;

                auto& data = dataSection.value().get();
                if (!data.contains(identifier)) {
                    _Log_(
                        "File {} does not contain a record with identifier '{}'", _path.string(),
                        identifier
                    );
                    return nullptr;
                }

                auto emplaced = _records.try_emplace(
                    identifier, std::make_unique<JsonRecord>(identifier, data[identifier])
                );
                if (!emplaced.second) {
                    _Log_(
                        "Failed to emplace record with identifier '{}' into file {}", identifier,
                        _path.string()
                    );
                    return nullptr;
                } else {
                    return emplaced.first->second.get();
                }
            }
        }

        //     std::vector<std::unique_ptr<DataEntry>> GetEntries() override {
        //         std::vector<std::unique_ptr<DataEntry>> entries;

        //         auto& fileData = LoadJson()["data"];
        //         for (auto& [key, value] : fileData.items())
        //             entries.emplace_back(std::move(std::make_unique<JsonDataEntry>(key, value)));

        //         return entries;
        // }
    };
}
