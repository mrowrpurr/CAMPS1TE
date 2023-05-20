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
        AssertThat(data->HasNumber("topLevelInteger"), IsTrue());
        AssertThat(data->HasFloat("topLevelFloat"), IsTrue());
        AssertThat(data->HasNumber("topLevelFloat"), IsTrue());
        AssertThat(data->HasList("topLevelStringArray"), IsTrue());
        AssertThat(data->HasMap("topLevelSimpleStringMap"), IsTrue());
        AssertThat(data->HasObject("topLevelSimpleStringMap"), IsTrue());
        AssertThat(data->HasObject("topLevelStringArray"), IsTrue());

        // Top Level - Negative
        AssertThat(data->Has("doesNotExist"), IsFalse());
        AssertThat(data->HasString("doesNotExist"), IsFalse());
        AssertThat(data->HasString("topLevelBool"), IsFalse());
        AssertThat(data->HasBool("doesNotExist"), IsFalse());
        AssertThat(data->HasBool("topLevelString"), IsFalse());
        AssertThat(data->HasNumber("doesNotExist"), IsFalse());
        AssertThat(data->HasNumber("topLevelString"), IsFalse());
        AssertThat(data->HasInt("doesNotExist"), IsFalse());
        AssertThat(data->HasInt("topLevelString"), IsFalse());
        AssertThat(data->HasFloat("doesNotExist"), IsFalse());
        AssertThat(data->HasFloat("topLevelString"), IsFalse());
        AssertThat(data->HasList("doesNotExist"), IsFalse());
        AssertThat(data->HasList("topLevelString"), IsFalse());
        AssertThat(data->HasMap("doesNotExist"), IsFalse());
        AssertThat(data->HasMap("topLevelStringArray"), IsFalse());
        AssertThat(data->HasObject("doesNotExist"), IsFalse());
        AssertThat(data->HasObject("topLevelString"), IsFalse());

        // Nested - Positive
        AssertThat(data->Has("a.few.nested.levels.nestedString"), IsTrue());
        AssertThat(data->HasString("a.few.nested.levels.nestedString"), IsTrue());
        AssertThat(data->HasBool("a.few.nested.levels.nestedBoolean"), IsTrue());
        AssertThat(data->HasInt("a.few.nested.levels.nestedInteger"), IsTrue());
        AssertThat(data->HasFloat("a.few.nested.levels.nestedFloat"), IsTrue());
        AssertThat(data->HasList("a.few.nested.levels.nestedStringArray"), IsTrue());
        AssertThat(data->HasMap("a.few.nested.levels.nestedSimpleStringMap"), IsTrue());
        AssertThat(data->HasObject("a.few.nested.levels.nestedStringArray"), IsTrue());
        AssertThat(data->HasObject("a.few.nested.levels.nestedSimpleStringMap"), IsTrue());

        // Nested - Negative
        AssertThat(data->Has("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasString("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasString("a.few.nested.levels.nestedBoolean"), IsFalse());
        AssertThat(data->HasBool("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasBool("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasInt("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasInt("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasFloat("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasFloat("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasList("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasList("a.few.nested.levels.nestedString"), IsFalse());
        AssertThat(data->HasList("a.few.nested.levels.nestedSimpleStringMap"), IsFalse());
        AssertThat(data->HasMap("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasMap("a.few.nested.levels.nestedStringArray"), IsFalse());
        AssertThat(data->HasObject("a.few.nested.levels.doesNotExist"), IsFalse());
        AssertThat(data->HasObject("a.few.nested.levels.nestedString"), IsFalse());
    });

    it("Has*At", []() {
        auto dataStore = GetJsonDataStore("DataFileForJsonRecordDataSpec.json");
        auto data      = dataStore->GetRecord("Cool", "Record1")->GetData();
        auto list      = data->GetObject("topLevelMiscStuffList").value();

        // Positive
        AssertThat(list->HasStringAt(0), IsTrue());
        AssertThat(list->HasIntAt(1), IsTrue());
        AssertThat(list->HasFloatAt(2), IsTrue());
        AssertThat(list->HasBoolAt(3), IsTrue());
        AssertThat(list->IsNullAt(4), IsTrue());
        AssertThat(list->HasMapAt(5), IsTrue());
        AssertThat(list->HasObjectAt(6), IsTrue());
        AssertThat(list->HasListAt(7), IsTrue());
        AssertThat(list->HasObjectAt(8), IsTrue());

        // Is an integer a valid float?
        AssertThat(list->HasFloatAt(1), IsFalse());  // FALSE!
        AssertThat(list->HasNumberAt(1), IsTrue());  // TRUE!

        // Is a float a valid integer?
        AssertThat(list->HasIntAt(2), IsFalse());    // FALSE!
        AssertThat(list->HasNumberAt(2), IsTrue());  // FALSE!

        // Negative
        AssertThat(list->HasStringAt(1), IsFalse());
        AssertThat(list->HasIntAt(0), IsFalse());
        AssertThat(list->HasFloatAt(1), IsFalse());
        AssertThat(list->HasBoolAt(0), IsFalse());
        AssertThat(list->IsNullAt(0), IsFalse());
        AssertThat(list->HasMapAt(0), IsFalse());
        AssertThat(list->HasObjectAt(0), IsFalse());
        AssertThat(list->HasListAt(0), IsFalse());
        AssertThat(list->HasObjectAt(0), IsFalse());
    });

    describe("Get*", []() {
        it("GetString", []() {
            auto dataStore = GetJsonDataStore("DataFileForJsonRecordDataSpec.json");
            auto data      = dataStore->GetRecord("Cool", "Record1")->GetData();
            AssertThat(data->GetString("DoesNotExist").has_value(), IsFalse());
            AssertThat(*data->GetString("topLevelString"), Equals("hello!"));
            AssertThat(*data->GetString("a.few.nested.levels.nestedString"), Equals("nested!"));
        });

        it("GetBool", []() {
            auto dataStore = GetJsonDataStore("DataFileForJsonRecordDataSpec.json");
            auto data      = dataStore->GetRecord("Cool", "Record1")->GetData();
            AssertThat(data->GetBool("DoesNotExist").has_value(), IsFalse());
            AssertThat(*data->GetBool("topLevelBoolean"), IsTrue());
            AssertThat(*data->GetBool("topFalseLevelBoolean"), IsFalse());
            AssertThat(*data->GetBool("a.few.nested.levels.nestedBoolean"), IsTrue());
        });

        it("GetInt", []() {
            auto dataStore = GetJsonDataStore("DataFileForJsonRecordDataSpec.json");
            auto data      = dataStore->GetRecord("Cool", "Record1")->GetData();
            AssertThat(data->GetInt("DoesNotExist").has_value(), IsFalse());
            AssertThat(*data->GetInt("topLevelInteger"), Equals(69));
            AssertThat(*data->GetInt("a.few.nested.levels.nestedInteger"), Equals(420));

            // Get a Float as an Int
            AssertThat(*data->GetInt("topLevelFloat"), Equals(123));

            // Strings aren't converted to numbers magically tho
            AssertThat(*data->GetInt("topLevelStringNumber"), Equals(0));
        });

        it("GetFloat", []() {
            auto dataStore = GetJsonDataStore("DataFileForJsonRecordDataSpec.json");
            auto data      = dataStore->GetRecord("Cool", "Record1")->GetData();
            AssertThat(data->GetFloat("DoesNotExist").has_value(), IsFalse());
            AssertThat(*data->GetFloat("topLevelFloat"), Equals(123.456f));
            AssertThat(*data->GetFloat("a.few.nested.levels.nestedFloat"), Equals(69420.123f));

            // Get an Int as a Float
            AssertThat(*data->GetFloat("topLevelInteger"), Equals(69.0f));

            // Strings aren't converted to numbers magically tho
            AssertThat(*data->GetFloat("topLevelStringNumber"), Equals(0.0f));
        });

        // TODO
    });

    // describe("Lists", []() {
    // });
}
