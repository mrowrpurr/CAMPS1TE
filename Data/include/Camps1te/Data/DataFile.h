#pragma once

#include <_Log_.h>

#include <memory>
#include <string>
#include <vector>

#include "DataEntry.h"


namespace Camps1te::Data {

    struct DataFile {
        virtual ~DataFile()                                          = default;
        virtual std::vector<std::unique_ptr<DataEntry>> GetEntries() = 0;
    };
}
