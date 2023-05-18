#pragma once

#include <string_format.h>

#include <vector>

#include "DataStore.h"
#include "JsonDataFile.h"
#include "JsonRecord.h"

namespace Camps1te::Data {

    class JsonDataStore : public DataStore {
        std::unordered_map<std::string, std::unique_ptr<nlohmann::json>> _recordJsons;
        std::unordered_map<std::string, std::unique_ptr<JsonRecord>>     _records;

        void MergeRecursive(nlohmann::json& destination, const nlohmann::json& source) {
            for (auto& item : source.items()) {
                std::string    key   = item.key();
                nlohmann::json value = item.value();

                std::string::size_type pos = key.rfind("::");
                std::string            action;
                if (pos != std::string::npos) {
                    action = key.substr(pos + 2);
                    key    = key.substr(0, pos);
                }

                if (action == "delete") {
                    destination.erase(key);
                } else if (action == "replace") {
                    destination[key] = value;
                } else if (action == "append") {
                    if (destination[key].is_array() && value.is_array()) {
                        destination[key].insert(destination[key].end(), value.begin(), value.end());
                    } else {
                        throw std::runtime_error("::append used on non-array values");
                    }
                } else if (action == "prepend") {
                    if (destination[key].is_array() && value.is_array()) {
                        destination[key].insert(
                            destination[key].begin(), value.begin(), value.end()
                        );
                    } else {
                        throw std::runtime_error("::prepend used on non-array values");
                    }
                } else {
                    if (destination.contains(key) && destination[key].is_object() &&
                        value.is_object()) {
                        MergeRecursive(destination[key], value);
                    } else {
                        destination[key] = value;
                    }
                }
            }
        }

        void MergeRecord(JsonRecord& jsonRecord) {
            auto  fullIdentifier = jsonRecord.GetFullIdentifier();
            auto& incomingJson   = jsonRecord.GetJsonDocument();
            auto& existingJson   = _recordJsons[fullIdentifier];
            MergeRecursive(*existingJson, incomingJson);
        }

    public:
        bool    InsertRecord(Record* record) override { return {}; }
        Record* GetRecord(const char* fullIdentifier) override {
            auto found = _records.find(fullIdentifier);
            if (found != _records.end()) return found->second.get();
            else return nullptr;
        }
        Record* GetRecord(const char* ownerName, const char* relativeIdentifier) override {
            return GetRecord(string_format("{}.{}", ownerName, relativeIdentifier).c_str());
        }
        std::vector<Record*> GetAllRecords() override {
            std::vector<Record*> records;
            for (auto& record : _records) records.push_back(record.second.get());
            return records;
        }
        std::vector<Record*> GetAllRecordsOfOwner(const char* ownerName) override { return {}; }
        std::vector<Record*> GetAllRecordsOfType(const char* typeName) override { return {}; }

        bool InsertRecord(JsonRecord& jsonRecord) {
            auto fullIdentifier = jsonRecord.GetFullIdentifier();
            if (_records.contains(fullIdentifier)) {
                MergeRecord(jsonRecord);
                return true;
            }

            auto jsonDocument            = jsonRecord.GetJsonDocument();
            _recordJsons[fullIdentifier] = std::make_unique<nlohmann::json>(jsonDocument);
            _records[fullIdentifier]     = std::make_unique<JsonRecord>(
                jsonRecord.GetOwnerName(), jsonRecord.GetRelativeIdentifier(),
                *_recordJsons[fullIdentifier].get()
            );
            return true;
        }

        bool InsertDataFile(JsonDataFile& jsonFile) {
            auto records = jsonFile.GetAllRecords();
            for (auto& record : records) {
                auto jsonRecord = static_cast<JsonRecord*>(record);
                InsertRecord(*jsonRecord);
            }
            return true;
        }
    };
}
