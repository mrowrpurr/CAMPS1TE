#pragma once

#include <_Log_.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "DataFile.h"
#include "JsonRecord.h"

namespace Camps1te::Data {

    namespace JsonDataFileKeys {
        constexpr const char* Data = "data";
    }

    class JsonDataFile : public DataFile {
        std::filesystem::path                                        _path;
        nlohmann::json                                               _json;
        std::unordered_map<std::string, std::unique_ptr<JsonRecord>> _records;

        std::vector<std::string> GetKeys(nlohmann::json jsonObject) {
            std::vector<std::string> keys;
            for (auto& [key, value] : jsonObject.items()) keys.push_back(key);
            return keys;
        }

    public:
        JsonDataFile(const std::filesystem::path& path) : _path(path) {}

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
            if (!json.contains(JsonDataFileKeys::Data)) {
                _Log_(
                    "File {} does not contain a '{}' section", _path.string(),
                    JsonDataFileKeys::Data
                );
                return std::nullopt;
            }
            return std::ref(json[JsonDataFileKeys::Data]);
        }

        std::string GetIdentifier() override { return _path.stem().string(); }

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
                    identifier, std::make_unique<JsonRecord>(
                                    GetIdentifier().c_str(), identifier, data[identifier]
                                )
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

        std::vector<Record*> GetAllRecords() override {
            std::vector<Record*> records;
            auto                 dataSection = GetDataSection();
            if (!dataSection.has_value()) {
                _Log_(
                    "File {} does not contain a '{}' section", _path.string(),
                    JsonDataFileKeys::Data
                );
                return records;
            }

            auto& data = dataSection.value().get();
            for (auto& [key, value] : data.items()) {
                auto record = GetRecord(key.c_str());
                if (record != nullptr) records.push_back(record);
            }

            return records;
        }
    };
}
