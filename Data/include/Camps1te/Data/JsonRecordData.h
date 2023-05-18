#pragma once

#include <_Log_.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "RecordData.h"

namespace Camps1te::Data {

    class JsonRecordData : public RecordData {
        nlohmann::json& _recordData;

    public:
        JsonRecordData(nlohmann::json& recordData) : _recordData(recordData) {}

        const char*                GetRecordPath() const { return nullptr; }
        bool                       HasChildRecord(const char* path) const { return false; }
        bool                       HasString(const char* path) const { return false; }
        bool                       HasBool(const char* path) const { return false; }
        bool                       HasInt(const char* path) const { return false; }
        bool                       HasFloat(const char* path) const { return false; }
        std::optional<RecordData*> GetChildRecord(const char* path) const { return std::nullopt; }
        std::optional<std::string> GetString(const char* path) const {
            if (_recordData.is_null()) return std::nullopt;
            if (!_recordData.contains(path)) return std::nullopt;
            return _recordData[path].get<std::string>();
        }
        std::optional<bool>        GetBool(const char* path) const { return std::nullopt; }
        std::optional<int>         GetInt(const char* path) const { return std::nullopt; }
        std::optional<float>       GetFloat(const char* path) const { return std::nullopt; }
        std::optional<RecordData*> GetChildRecordAt(unsigned int index) const {
            return std::nullopt;
        }
        std::optional<std::string> GetStringAt(unsigned int index) const { return std::nullopt; }
        std::optional<bool>        GetBoolAt(unsigned int index) const { return std::nullopt; }
        std::optional<int>         GetIntAt(unsigned int index) const { return std::nullopt; }
        std::optional<float>       GetFloatAt(unsigned int index) const { return std::nullopt; }
    };
}
