#pragma once

#include <string>

namespace Camps1te::Data {

    struct DataEntry {
        virtual ~DataEntry()                                         = default;
        virtual std::string GetIdentifier() const                    = 0;
        virtual std::string GetType() const                          = 0;
        virtual std::string GetString(const std::string& path) const = 0;
    };
}
