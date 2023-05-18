#pragma once

#include <_Log_.h>
#include <string_format.h>

#include <nlohmann/json.hpp>

#include "JsonRecordData.h"
#include "Record.h"

namespace Camps1te::Data {

    class JsonRecord : public Record {
        bool                            _isDeletion;
        std::string                     _owner;
        std::string                     _identifier;
        std::string                     _fullIdentifier;
        std::string                     _type;
        nlohmann::json&                 _recordJson;
        std::unique_ptr<JsonRecordData> _recordData;

        std::optional<std::reference_wrapper<nlohmann::json>> GetJsonDataSection() {
            if (_recordJson.is_null()) return std::nullopt;
            if (!_recordJson.contains("data")) {
                _Log_("Record {} does not contain a 'data' section", _identifier);
                return std::nullopt;
            }
            return std::ref(_recordJson["data"]);
        }

    public:
        JsonRecord(
            const std::string& owner, const std::string& identifier, nlohmann::json& recordData
        )
            : _owner(owner), _identifier(identifier), _recordJson(recordData) {
            std::string id{identifier};

            if (id.find("::delete") != std::string::npos) {
                _isDeletion = true;
                id          = id.substr(0, _identifier.find("::delete"));
            } else {
                _isDeletion = false;
            }

            if (id.find('.') != std::string::npos) _fullIdentifier = id;
            else _fullIdentifier = string_format("{}.{}", _owner, id);
        }

        const char* GetFullIdentifier() override { return _fullIdentifier.c_str(); }
        const char* GetOwnerName() override { return _owner.c_str(); }
        const char* GetRelativeIdentifier() override { return _identifier.c_str(); }
        const char* GetType() override {
            if (!_type.empty()) return _type.c_str();
            if (_recordJson.is_null()) return nullptr;
            if (!_recordJson.contains("type")) {
                _Log_("Record {} does not contain a 'type' section", _identifier);
                return nullptr;
            }
            _type = _recordJson["type"].get<std::string>().c_str();
            return _type.c_str();
        }
        bool IsDeletion() override { return _isDeletion; }

        //

        nlohmann::json& GetJsonDocument() { return _recordJson; }

        RecordData* GetData() override {
            if (_recordData) return _recordData.get();
            auto dataSection = GetJsonDataSection();
            if (!dataSection.has_value()) return nullptr;
            _recordData = std::make_unique<JsonRecordData>(dataSection.value().get());
            return _recordData.get();
        }
    };
}
