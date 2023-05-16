#pragma once

#include "DataFile.h"

namespace Camps1te::Data {

    struct DataStore {
        virtual ~DataStore()                          = default;
        virtual bool LoadDataFile(DataFile& dataFile) = 0;
    };
}
