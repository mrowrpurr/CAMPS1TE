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
        AssertThat(data->Has("a.few.nested.levels.nestedString.XXX"), IsTrue());
        // AssertThat(data->HasString(""), IsTrue());
        // AssertThat(data->HasBool(""), IsTrue());
        // AssertThat(data->HasInt(""), IsTrue());
        // AssertThat(data->HasFloat(""), IsTrue());
        // AssertThat(data->HasList(""), IsTrue());
        // AssertThat(data->HasRecord(""), IsTrue());
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
