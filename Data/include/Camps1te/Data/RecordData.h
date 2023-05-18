#pragma once

#include <optional>
#include <string>

namespace Camps1te::Data {

    struct RecordData {
        virtual ~RecordData() = default;

        virtual const char* GetRecordPath() const = 0;

        virtual bool HasChildRecord(const char* path) const = 0;
        virtual bool HasString(const char* path) const      = 0;
        virtual bool HasBool(const char* path) const        = 0;
        virtual bool HasInt(const char* path) const         = 0;
        virtual bool HasFloat(const char* path) const       = 0;

        virtual std::optional<RecordData*> GetChildRecord(const char* path) const = 0;
        virtual std::optional<std::string> GetString(const char* path) const      = 0;
        virtual std::optional<bool>        GetBool(const char* path) const        = 0;
        virtual std::optional<int>         GetInt(const char* path) const         = 0;
        virtual std::optional<float>       GetFloat(const char* path) const       = 0;

        virtual std::optional<RecordData*> GetChildRecordAt(unsigned int index) const = 0;
        virtual std::optional<std::string> GetStringAt(unsigned int index) const      = 0;
        virtual std::optional<bool>        GetBoolAt(unsigned int index) const        = 0;
        virtual std::optional<int>         GetIntAt(unsigned int index) const         = 0;
        virtual std::optional<float>       GetFloatAt(unsigned int index) const       = 0;
    };
}
