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
        virtual bool HasNumber(const char* path) const = 0;
        virtual bool HasInt(const char* path) const    = 0;
        virtual bool HasFloat(const char* path) const  = 0;
        virtual bool HasList(const char* path) const   = 0;
        virtual bool HasMap(const char* path) const    = 0;
        virtual bool HasObject(const char* path) const = 0;
        virtual bool IsNull(const char* path) const    = 0;

        virtual bool HasAt(size_t index) const       = 0;
        virtual bool HasStringAt(size_t index) const = 0;
        virtual bool HasBoolAt(size_t index) const   = 0;
        virtual bool HasNumberAt(size_t index) const = 0;
        virtual bool HasIntAt(size_t index) const    = 0;
        virtual bool HasFloatAt(size_t index) const  = 0;
        virtual bool HasListAt(size_t index) const   = 0;
        virtual bool HasMapAt(size_t index) const    = 0;
        virtual bool HasObjectAt(size_t index) const = 0;
        virtual bool IsNullAt(size_t index) const    = 0;

        virtual size_t                   GetListCount(const char* path) const = 0;
        virtual std::vector<std::string> GetMapKeys(const char* path) const   = 0;

        virtual std::optional<std::string>              GetString(const char* path) const     = 0;
        virtual std::optional<bool>                     GetBool(const char* path) const       = 0;
        virtual std::optional<int>                      GetInt(const char* path) const        = 0;
        virtual std::optional<float>                    GetFloat(const char* path) const      = 0;
        virtual std::optional<RecordData*>              GetObject(const char* path) const     = 0;
        virtual std::optional<std::vector<std::string>> GetStringList(const char* path) const = 0;
        virtual std::optional<std::vector<bool>>        GetBoolList(const char* path) const   = 0;
        virtual std::optional<std::vector<int>>         GetIntList(const char* path) const    = 0;
        virtual std::optional<std::vector<float>>       GetFloatList(const char* path) const  = 0;
        virtual std::optional<std::vector<RecordData*>> GetObjectList(const char* path) const = 0;

        virtual std::optional<std::string>              GetStringAt(size_t index) const     = 0;
        virtual std::optional<bool>                     GetBoolAt(size_t index) const       = 0;
        virtual std::optional<int>                      GetIntAt(size_t index) const        = 0;
        virtual std::optional<float>                    GetFloatAt(size_t index) const      = 0;
        virtual std::optional<RecordData*>              GetObjectAt(size_t index) const     = 0;
        virtual std::optional<std::vector<std::string>> GetStringListAt(size_t index) const = 0;
        virtual std::optional<std::vector<bool>>        GetBoolListAt(size_t index) const   = 0;
        virtual std::optional<std::vector<int>>         GetIntListAt(size_t index) const    = 0;
        virtual std::optional<std::vector<float>>       GetFloatListAt(size_t index) const  = 0;
        virtual std::optional<std::vector<RecordData*>> GetObjectListAt(size_t index) const = 0;
    };
}
