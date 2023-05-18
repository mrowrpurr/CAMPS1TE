#pragma once

#include <_Log_.h>

#include <nlohmann/json.hpp>

#include "JsonRecordData.h"
#include "Record.h"

namespace Camps1te::Data {

    class JsonRecord : public Record {
        const char*                     _identifier;
        nlohmann::json&                 _recordJson;
        std::unique_ptr<JsonRecordData> _recordData;

        std::optional<std::reference_wrapper<nlohmann::json>> GetDataSection() {
            if (_recordJson.is_null()) return std::nullopt;
            if (!_recordJson.contains("data")) {
                _Log_("Record {} does not contain a 'data' section", _identifier);
                return std::nullopt;
            }
            return std::ref(_recordJson["data"]);
        }

    public:
        JsonRecord(const char* identifier, nlohmann::json& recordData)
            : _identifier(identifier), _recordJson(recordData) {}

        const char* GetFullIdentifier() override { return nullptr; }
        const char* GetOwnerName() override { return nullptr; }
        const char* GetRelativeIdentifier() override { return nullptr; }
        const char* GetType() override { return nullptr; }

        RecordData* GetData() override {
            if (_recordData) return _recordData.get();
            auto dataSection = GetDataSection();
            if (!dataSection.has_value()) return nullptr;
            _recordData = std::make_unique<JsonRecordData>(dataSection.value().get());
            return _recordData.get();
        }
    };
}
