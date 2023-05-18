#pragma once

#include <vector>

#include "../DataStore.h"
#include "JsonRecord.h"

namespace Camps1te::Data {

    struct JsonDataStore : public DataStore {
        bool InsertRecord(Record* record) override { return false; }

        Record* GetRecord(const char* fullIdentifier) override { return nullptr; }
        Record* GetRecord(const char* ownerName, const char* relativeIdentifier) override {
            return nullptr;
        }

        std::vector<Record*> GetAllRecords() override { return std::vector<Record*>(); }
        std::vector<Record*> GetAllRecordsOfOwner(const char* ownerName) override {
            return std::vector<Record*>();
        }
        std::vector<Record*> GetAllRecordsOfType(const char* typeName) override {
            return std::vector<Record*>();
        }
    };
}
