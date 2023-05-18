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

        bool InsertDataFile(JsonDataFile& jsonFile) {
            auto records = jsonFile.GetAllRecords();
            for (auto& record : records) {
                auto fullIdentifier = record->GetFullIdentifier();
                if (_records.contains(fullIdentifier)) {
                    // Do things...
                    _Log_("Duplicate record found: {}", fullIdentifier);
                    return false;
                }
                auto jsonRecord              = static_cast<JsonRecord*>(record);
                auto jsonDocument            = jsonRecord->GetJsonDocument();
                _recordJsons[fullIdentifier] = std::make_unique<nlohmann::json>(jsonDocument);
                _records[fullIdentifier]     = std::make_unique<JsonRecord>(
                    jsonRecord->GetOwnerName(), jsonRecord->GetRelativeIdentifier(),
                    *_recordJsons[fullIdentifier].get()
                );
            }

            return true;
        }
    };
}
