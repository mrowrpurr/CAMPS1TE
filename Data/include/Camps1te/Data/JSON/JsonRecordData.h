#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "../RecordData.h"

namespace Camps1te::Data {

    struct JsonRecordData : public RecordData {
        const char* GetRecordPath() const override { return nullptr; }

        bool HasChildRecord(const char* path) const override { return false; }
        bool HasString(const char* path) const override { return false; }
        bool HasBool(const char* path) const override { return false; }
        bool HasInt(const char* path) const override { return false; }
        bool HasFloat(const char* path) const override { return false; }

        std::optional<RecordData*> GetChildRecord(const char* path) const override {
            return std::nullopt;
        }
        std::optional<const char*> GetString(const char* path) const override {
            return std::nullopt;
        }
        std::optional<bool>  GetBool(const char* path) const override { return std::nullopt; }
        std::optional<int>   GetInt(const char* path) const override { return std::nullopt; }
        std::optional<float> GetFloat(const char* path) const override { return std::nullopt; }

        std::optional<RecordData*> GetChildRecordAt(unsigned int index) const override {
            return std::nullopt;
        }
        std::optional<const char*> GetStringAt(unsigned int index) const override {
            return std::nullopt;
        }
        std::optional<bool>  GetBoolAt(unsigned int index) const override { return std::nullopt; }
        std::optional<int>   GetIntAt(unsigned int index) const override { return std::nullopt; }
        std::optional<float> GetFloatAt(unsigned int index) const override { return std::nullopt; }
    };
}
