#include <nlohmann/json.hpp>

#include "Camps1te/Data/DataFile.h"
#include "Camps1te/Data/DataStore.h"
#include "Camps1te/Data/JsonDataFile.h"
#include "Camps1te/Data/JsonDataStore.h"
#include "Camps1te/Data/JsonRecord.h"
#include "Camps1te/Data/JsonRecordData.h"
#include "_SpecHelper.h"

Describe("JsonRecordData") {
    it("Has*", []() {
        auto dataStore = GetJsonDataStore("DataFileForJsonRecordDataSpec.json");
        auto data      = dataStore->GetRecord("Cool", "Record1")->GetData();

        // Top Level - Positive
        AssertThat(data->Has("topLevelString"), IsTrue());
        AssertThat(data->HasString("topLevelString"), IsTrue());
        AssertThat(data->HasBool("topLevelBoolean"), IsTrue());
        AssertThat(data->HasInt("topLevelInteger"), IsTrue());
        AssertThat(data->HasFloat("topLevelFloat"), IsTrue());
        AssertThat(data->HasList("topLevelStringArray"), IsTrue());
        AssertThat(data->HasRecord("topLevelSimpleStringMap"), IsTrue());

        // Top Level - Negative
        AssertThat(data->Has("doesNotExist"), IsFalse());
        AssertThat(data->HasString("doesNotExist"), IsFalse());
        AssertThat(data->HasString("topLevelBool"), IsFalse());
        AssertThat(data->HasBool("doesNotExist"), IsFalse());
        AssertThat(data->HasBool("topLevelString"), IsFalse());
        AssertThat(data->HasInt("doesNotExist"), IsFalse());
        AssertThat(data->HasInt("topLevelString"), IsFalse());
        AssertThat(data->HasFloat("doesNotExist"), IsFalse());
        AssertThat(data->HasFloat("topLevelString"), IsFalse());
        AssertThat(data->HasList("doesNotExist"), IsFalse());
        AssertThat(data->HasList("topLevelString"), IsFalse());
        AssertThat(data->HasRecord("doesNotExist"), IsFalse());
        AssertThat(data->HasRecord("topLevelString"), IsFalse());

        // Nested - Positive
        AssertThat(data->Has("a.few.nested.levels.nestedString"), IsTrue());
        AssertThat(data->HasString("a.few.nested.levels.nestedString"), IsTrue());
        AssertThat(data->HasBool("a.few.nested.levels.nestedBoolean"), IsTrue());
        AssertThat(data->HasInt("a.few.nested.levels.nestedInteger"), IsTrue());
        AssertThat(data->HasFloat("a.few.nested.levels.nestedFloat"), IsTrue());
        AssertThat(data->HasList("a.few.nested.levels.nestedIntegerArray"), IsTrue());
        AssertThat(data->HasRecord("a.few.nested.levels.nestedSimpleStringMap"), IsTrue());

        // Nested - Negative
        AssertThat(data->Has("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasString("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasString("a.few.nested.levels.nestedBool"), IsFalse());
        AssertThat(data->HasBool("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasBool("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasInt("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasInt("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasFloat("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasFloat("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasList("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasList("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasRecord("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasRecord("a.few.nested.levels.nestedString"), IsFalse());
    });

    it("Has*At", []() {
        //
    });

    describe("Get*", []() {
        //
        it("GetString", []() {
            //
        });
    });

    describe("Lists", []() {
        //
        it("x", []() {
            // ...
        });
    });
}
