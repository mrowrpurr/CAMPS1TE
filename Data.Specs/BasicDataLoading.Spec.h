#include <memory>

#include "Camps1te/Data/DataFile.h"
#include "Camps1te/Data/JsonDataFile.h"
#include "Camps1te/Data/JsonRecord.h"
#include "Camps1te/Data/JsonRecordData.h"
#include "_SpecHelper.h"

using namespace Camps1te::Data;

Describe("Basic Data Loading") {
    it("can load and read *something* from a data file", []() {
        auto jsonFilePath = GetResourcePath("FoodDataFile.json");
        auto dataFile     = std::unique_ptr<DataFile>(new JsonDataFile{jsonFilePath});

        // ASSERT that we can read some stuff!
        auto sweetroll = dataFile->GetRecord("sweetroll");
        AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
        AssertThat(
            sweetroll->GetData()->GetString("description").value(), Contains("sweet baked dessert")
        );
    });
}
