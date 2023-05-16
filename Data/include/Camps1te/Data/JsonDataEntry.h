#pragma once

#include <nlohmann/json.hpp>

#include "DataEntry.h"

namespace Camps1te::Data {

    class JsonDataEntry : public DataEntry {
        std::string     _identifier;
        nlohmann::json& _json;

    public:
        JsonDataEntry(const std::string& identifier, nlohmann::json& json)
            : _identifier(identifier), _json(json) {}

        std::string GetType() const override { return _json["type"]; }
        std::string GetIdentifier() const override { return _identifier; }
    };
}
