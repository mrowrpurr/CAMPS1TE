#pragma once

#include <Specs/BDD.h>
#include <snowhouse/snowhouse.h>

#include <filesystem>
#include <functional>
#include <memory>

#include "Camps1te/Data/DataFile.h"
#include "Camps1te/Data/DataStore.h"
#include "Camps1te/Data/JsonDataFile.h"
#include "Camps1te/Data/JsonDataStore.h"
#include "Camps1te/Data/JsonRecord.h"
#include "Camps1te/Data/JsonRecordData.h"

using namespace snowhouse;
using namespace Camps1te::Data;

std::filesystem::path SpecResourcesFolder{"../../../../Data.Specs/Resources"};

std::filesystem::path GetResourcePath(const std::string& path) {
    return SpecResourcesFolder / path;
}

std::unique_ptr<DataFile> GetDataFile(const std::string& fileName) {
    auto jsonFilePath = GetResourcePath(fileName);
    return std::unique_ptr<DataFile>(new JsonDataFile{jsonFilePath});
}

std::unique_ptr<JsonDataFile> GetJsonDataFile(const std::string& fileName) {
    auto jsonFilePath = GetResourcePath(fileName);
    return std::unique_ptr<JsonDataFile>(new JsonDataFile{jsonFilePath});
}

std::unique_ptr<JsonDataStore> GetJsonDataStore(const std::string& fileName) {
    auto dataFile  = GetJsonDataFile(fileName);
    auto dataStore = new JsonDataStore();
    dataStore->InsertDataFile(*dataFile);
    return std::unique_ptr<JsonDataStore>(dataStore);
}

#define Assert "Do not use Assert::That"

namespace HacksPendingSpecsCppLibraryCompletion {
    std::vector<std::function<void()>> SpecDefinitionBodies;

    struct CollectSpecDefinitionBody {
        CollectSpecDefinitionBody(std::function<void()> body) {
            SpecDefinitionBodies.push_back(body);
        }
    };
}

#define __SpecsHack_Macro_Concat_(x, y) x##y
#define __SpecsHack_Macro_Concat(x, y) __SpecsHack_Macro_Concat_(x, y)
#define __SpecsHack_Macro_Unique(count) \
    __SpecsHack_Macro_Concat(__specsMacroCreatedVariable__, count)

#define __SpecsHack_Macro_LambdaCollector(count)                                               \
    HacksPendingSpecsCppLibraryCompletion::CollectSpecDefinitionBody __SpecsHack_Macro_Concat( \
        __specsMacroCreatedFunction__, count                                                   \
    )

#define __SpecsHack_Macro_CollectSpecs(count)                                              \
    inline void __SpecsHack_Macro_Unique(count)();                                         \
    __SpecsHack_Macro_LambdaCollector(count){[]() { __SpecsHack_Macro_Unique(count)(); }}; \
    inline void __SpecsHack_Macro_Unique(count)()

#define Tests __SpecsHack_Macro_CollectSpecs(__COUNTER__)

#define __SpecsHack_Macro_DefineDescribe(description, count)                         \
    inline void __SpecsHack_Macro_Unique(count)();                                   \
    __SpecsHack_Macro_LambdaCollector(count                                          \
    ){[]() { describe(description, []() { __SpecsHack_Macro_Unique(count)(); }); }}; \
    inline void __SpecsHack_Macro_Unique(count)()

#define Describe(description) __SpecsHack_Macro_DefineDescribe(description, __COUNTER__)