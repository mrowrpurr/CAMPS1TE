#pragma once

#include <_Log_.h>

#include <optional>
#include <string>
#include <vector>

namespace Camps1te::Data {

    struct RecordData {
        virtual ~RecordData() = default;

        virtual bool Has(const char* path) const       = 0;
        virtual bool HasString(const char* path) const = 0;
        virtual bool HasBool(const char* path) const   = 0;
        virtual bool HasInt(const char* path) const    = 0;
        virtual bool HasFloat(const char* path) const  = 0;
        virtual bool HasList(const char* path) const   = 0;
        virtual bool HasRecord(const char* path) const = 0;
        virtual bool IsNull(const char* path) const    = 0;

        virtual bool HasAt(size_t index) const       = 0;
        virtual bool HasStringAt(size_t index) const = 0;
        virtual bool HasBoolAt(size_t index) const   = 0;
        virtual bool HasIntAt(size_t index) const    = 0;
        virtual bool HasFloatAt(size_t index) const  = 0;
        virtual bool HasListAt(size_t index) const   = 0;
        virtual bool HasRecordAt(size_t index) const = 0;
        virtual bool IsNullAt(size_t index) const    = 0;

        virtual size_t                   GetListCount(const char* path) const = 0;
        virtual std::vector<std::string> GetMapKeys(const char* path) const   = 0;

        virtual std::optional<std::string>              GetString(const char* path) const     = 0;
        virtual std::optional<bool>                     GetBool(const char* path) const       = 0;
        virtual std::optional<int>                      GetInt(const char* path) const        = 0;
        virtual std::optional<float>                    GetFloat(const char* path) const      = 0;
        virtual std::optional<RecordData*>              GetRecord(const char* path) const     = 0;
        virtual std::optional<std::vector<std::string>> GetStringList(const char* path) const = 0;
        virtual std::optional<std::vector<bool>>        GetBoolList(const char* path) const   = 0;
        virtual std::optional<std::vector<int>>         GetIntList(const char* path) const    = 0;
        virtual std::optional<std::vector<float>>       GetFloatList(const char* path) const  = 0;
        virtual std::optional<std::vector<RecordData*>> GetRecordList(const char* path) const = 0;

        virtual std::optional<std::string>              GetStringAt(size_t index) const     = 0;
        virtual std::optional<bool>                     GetBoolAt(size_t index) const       = 0;
        virtual std::optional<int>                      GetIntAt(size_t index) const        = 0;
        virtual std::optional<float>                    GetFloatAt(size_t index) const      = 0;
        virtual std::optional<RecordData*>              GetRecordAt(size_t index) const     = 0;
        virtual std::optional<std::vector<std::string>> GetStringListAt(size_t index) const = 0;
        virtual std::optional<std::vector<bool>>        GetBoolListAt(size_t index) const   = 0;
        virtual std::optional<std::vector<int>>         GetIntListAt(size_t index) const    = 0;
        virtual std::optional<std::vector<float>>       GetFloatListAt(size_t index) const  = 0;
        virtual std::optional<std::vector<RecordData*>> GetRecordListAt(size_t index) const = 0;

        template <typename T>
        std::optional<T> Get(const char* path) {
            if constexpr (std::is_same_v<T, RecordData*>) {
                return GetRecord(path);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return GetString(path);
            } else if constexpr (std::is_same_v<T, bool>) {
                return GetBool(path);
            } else if constexpr (std::is_same_v<T, int>) {
                return GetInt(path);
            } else if constexpr (std::is_same_v<T, float>) {
                return GetFloat(path);
            } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
                return GetStringList(path);
            } else if constexpr (std::is_same_v<T, std::vector<bool>>) {
                return GetBoolList(path);
            } else if constexpr (std::is_same_v<T, std::vector<int>>) {
                return GetIntList(path);
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                return GetFloatList(path);
            } else if constexpr (std::is_same_v<T, std::vector<RecordData*>>) {
                return GetRecordList(path);
            } else {
                _Log_("Unsupported type for RecordData.Get()");
                return std::nullopt;
            }
        }

        template <typename T>
        std::optional<T> GetAt(size_t index) {
            if constexpr (std::is_same_v<T, RecordData*>) {
                return GetRecordAt(index);
            } else if constexpr (std::is_same_v<T, std::string>) {
                return GetStringAt(index);
            } else if constexpr (std::is_same_v<T, bool>) {
                return GetBoolAt(index);
            } else if constexpr (std::is_same_v<T, int>) {
                return GetIntAt(index);
            } else if constexpr (std::is_same_v<T, float>) {
                return GetFloatAt(index);
            } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
                return GetStringListAt(index);
            } else if constexpr (std::is_same_v<T, std::vector<bool>>) {
                return GetBoolListAt(index);
            } else if constexpr (std::is_same_v<T, std::vector<int>>) {
                return GetIntListAt(index);
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                return GetFloatListAt(index);
            } else if constexpr (std::is_same_v<T, std::vector<RecordData*>>) {
                return GetRecordListAt(index);
            } else {
                _Log_("Unsupported type for RecordData.GetAt()");
                return std::nullopt;
            }
        }
    };
}
