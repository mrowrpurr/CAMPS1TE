#include "_SpecHelper.h"

Describe("JsonDataFile") {
    it("has identifier which matches name of file", []() {
        auto dataFile1 = GetDataFile("FoodDataFile.json");
        auto dataFile2 = GetDataFile("FoodSweetrollPatch.json");

        AssertThat(dataFile1->GetIdentifier(), Equals("FoodDataFile"));
        AssertThat(dataFile2->GetIdentifier(), Equals("FoodSweetrollPatch"));
    });

    it("can get and read individual records from data file using relative record IDs", []() {
        auto dataFile = GetDataFile("FoodDataFile.json");

        auto sweetroll = dataFile->GetRecord("sweetroll");

        AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
        AssertThat(
            sweetroll->GetData()->GetString("description").value(), Contains("sweet baked dessert")
        );
    });

    it("can get and read all records", []() {
        auto dataFile = GetDataFile("FoodDataFile.json");

        auto records = dataFile->GetAllRecords();
        AssertThat(records.size(), Equals(4));

        std::vector<std::string> recordIds;
        for (auto record : records) recordIds.push_back(record->GetFullIdentifier());

        AssertThat(recordIds, Contains("FoodDataFile.sweetroll"));
        AssertThat(recordIds, Contains("FoodDataFile.parfait"));
    });
}
