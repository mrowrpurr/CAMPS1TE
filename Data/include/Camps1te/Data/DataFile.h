#pragma once

#include "Record.h"

namespace Camps1te::Data {

    struct DataFile {
        virtual ~DataFile() = default;

        virtual Record* GetRecord(const char* identifier) = 0;
    };
}
