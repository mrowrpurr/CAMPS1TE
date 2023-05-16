#pragma once

#include <_Log_.h>

#include <string>
#include <vector>

namespace Camps1te::Data {

    struct DataFile {
        virtual ~DataFile()                                                          = default;
        virtual std::vector<std::string> GetPluginNames()                            = 0;
        virtual std::vector<std::string> GetDataTypes(const std::string& pluginName) = 0;
    };
}
