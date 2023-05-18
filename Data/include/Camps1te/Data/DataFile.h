#pragma once

#include <string>
#include <vector>

#include "Record.h"

namespace Camps1te::Data {

    struct DataFile {
        virtual ~DataFile() = default;

        virtual std::string          GetIdentifier()                   = 0;
        virtual Record*              GetRecord(const char* identifier) = 0;
        virtual std::vector<Record*> GetAllRecords()                   = 0;
    };
}
