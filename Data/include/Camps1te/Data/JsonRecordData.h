#pragma once

#include <_Log_.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "RecordData.h"

namespace Camps1te::Data {

    // TODO - there are lots of functions which get the nested json and then check the type and then
    // get it AGAIN update these to only get the value once

    class JsonRecordData : public RecordData {
        nlohmann::json& _recordData;

        std::optional<std::reference_wrapper<nlohmann::json>> GetNestedJson(const std::string& path
        ) const {
            std::istringstream path_stream(path);
            std::string        segment;
            nlohmann::json*    json = &_recordData;

            while (std::getline(path_stream, segment, '.')) {
                if (!json->contains(segment)) return std::nullopt;
                json = &(*json)[segment];
            }

            return *json;
        }

    public:
        JsonRecordData(nlohmann::json& recordData) : _recordData(recordData) {}

        bool Has(const char* path) const { return static_cast<bool>(GetNestedJson(path)); }

        bool HasString(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_string();
        }
        bool HasBool(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_boolean();
        }
        bool HasNumber(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_number();
        }
        bool HasInt(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_number_integer();
        }
        bool HasFloat(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_number_float();
        }
        bool HasList(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_array();
        }
        bool HasMap(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_object();
        }
        bool HasObject(const char* path) const {
            auto value = GetNestedJson(path);
            return value && (value->get().is_object() || value->get().is_array());
        }

        bool IsNull(const char* path) const {
            auto value = GetNestedJson(path);
            return value && value->get().is_null();
        }

        bool HasAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index;
        }
        bool HasStringAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_string();
        }
        bool HasBoolAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_boolean();
        }
        bool HasNumberAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_number();
        }
        bool HasIntAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_number_integer();
        }
        bool HasFloatAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_number_float();
        }
        bool HasListAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_array();
        }
        bool HasMapAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_object();
        }
        bool HasObjectAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index &&
                   (_recordData[index].is_object() || _recordData[index].is_array());
        }

        bool IsNullAt(size_t index) const {
            if (_recordData.is_null()) return false;
            return _recordData.size() > index && _recordData[index].is_null();
        }

        size_t GetListCount(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return 0;
            return value->get().size();
        }
        std::vector<std::string> GetMapKeys(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_object()) return {};
            std::vector<std::string> keys;
            for (auto& [key, value] : value->get().items()) keys.push_back(key);
            return keys;
        }

        std::optional<std::string> GetString(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_string()) return {};
            return value->get().get<std::string>();
        }
        std::optional<bool> GetBool(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_boolean()) return {};
            return value->get().get<bool>();
        }
        std::optional<int> GetInt(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_number()) return {};
            return value->get().get<int>();
        }
        std::optional<float> GetFloat(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_number()) return {};
            return value->get().get<float>();
        }
        std::optional<RecordData*> GetMap(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_object()) return {};
            return new JsonRecordData(value->get());
        }
        std::optional<RecordData*> GetList(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return {};
            return new JsonRecordData(value->get());
        }
        std::optional<RecordData*> GetObject(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !(value->get().is_object() || value->get().is_array())) return {};
            return new JsonRecordData(value->get());
        }
        std::optional<std::vector<std::string>> GetStringList(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return {};
            std::vector<std::string> list;
            for (auto& item : value->get()) {
                if (!item.is_string()) return {};
                list.push_back(item.get<std::string>());
            }
            return list;
        }
        std::optional<std::vector<bool>> GetBoolList(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return {};
            std::vector<bool> list;
            for (auto& item : value->get()) {
                if (!item.is_boolean()) return {};
                list.push_back(item.get<bool>());
            }
            return list;
        }
        std::optional<std::vector<int>> GetIntList(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return {};
            std::vector<int> list;
            for (auto& item : value->get()) {
                if (!item.is_number_integer()) return {};
                list.push_back(item.get<int>());
            }
            return list;
        }
        std::optional<std::vector<float>> GetFloatList(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return {};
            std::vector<float> list;
            for (auto& item : value->get()) {
                if (!item.is_number_float()) return {};
                list.push_back(item.get<float>());
            }
            return list;
        }
        std::optional<std::vector<RecordData*>> GetObjectList(const char* path) const {
            auto value = GetNestedJson(path);
            if (!value || !value->get().is_array()) return {};
            std::vector<RecordData*> list;
            for (auto& item : value->get()) {
                if (!item.is_object()) return {};
                list.push_back(new JsonRecordData(item));
            }
            return list;
        }

        std::optional<std::string> GetStringAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_string()) return {};
            return value.get<std::string>();
        }
        std::optional<bool> GetBoolAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_boolean()) return {};
            return value.get<bool>();
        }
        std::optional<int> GetIntAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_number()) return {};
            return value.get<int>();
        }
        std::optional<float> GetFloatAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_number()) return {};
            return value.get<float>();
        }
        std::optional<RecordData*> GetMapAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_object()) return {};
            return new JsonRecordData(value);
        }
        std::optional<RecordData*> GetListAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_array()) return {};
            return new JsonRecordData(value);
        }
        std::optional<RecordData*> GetObjectAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_object() && !value.is_array()) return {};
            return new JsonRecordData(value);
        }
        std::optional<std::vector<std::string>> GetStringListAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_array()) return {};
            std::vector<std::string> list;
            for (auto& item : value) {
                if (!item.is_string()) return {};
                list.push_back(item.get<std::string>());
            }
            return list;
        }
        std::optional<std::vector<bool>> GetBoolListAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_array()) return {};
            std::vector<bool> list;
            for (auto& item : value) {
                if (!item.is_boolean()) return {};
                list.push_back(item.get<bool>());
            }
            return list;
        }
        std::optional<std::vector<int>> GetIntListAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_array()) return {};
            std::vector<int> list;
            for (auto& item : value) {
                if (!item.is_number()) return {};
                list.push_back(item.get<int>());
            }
            return list;
        }
        std::optional<std::vector<float>> GetFloatListAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_array()) return {};
            std::vector<float> list;
            for (auto& item : value) {
                if (!item.is_number()) return {};
                list.push_back(item.get<float>());
            }
            return list;
        }
        std::optional<std::vector<RecordData*>> GetObjectListAt(size_t index) const {
            if (_recordData.is_null()) return {};
            if (_recordData.size() <= index) return {};
            auto value = _recordData[index];
            if (!value.is_array()) return {};
            std::vector<RecordData*> list;
            for (auto& item : value) {
                if (!item.is_object() && !item.is_array()) return {};
                list.push_back(new JsonRecordData(item));
            }
            return list;
        }
    };
}
