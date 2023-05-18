#include "_SpecHelper.h"

Describe("JsonDataStore") {
    it("can load a data file and get its records", []() {
        JsonDataStore dataStore;
        dataStore.InsertDataFile(*GetJsonDataFile("FoodDataFile.json"));

        auto sweetroll = dataStore.GetRecord("FoodDataFile.sweetroll");
        AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
        AssertThat(
            sweetroll->GetData()->GetString("description").value(), Contains("sweet baked dessert")
        );

        auto records = dataStore.GetAllRecords();
        AssertThat(records.size(), Equals(4));

        std::vector<std::string> recordIds;
        for (auto record : records) recordIds.push_back(record->GetFullIdentifier());

        AssertThat(recordIds, Contains("FoodDataFile.sweetroll"));
        AssertThat(recordIds, Contains("FoodDataFile.parfait"));
    });

    it("can load a data file which overwrites existing record data", []() {
        JsonDataStore dataStore;
        dataStore.InsertDataFile(*GetJsonDataFile("FoodDataFile.json"));

        auto sweetroll = dataStore.GetRecord("FoodDataFile.sweetroll");
        AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
        AssertThat(
            sweetroll->GetData()->GetString("description").value(), Contains("sweet baked dessert")
        );

        dataStore.InsertDataFile(*GetJsonDataFile("FoodSweetrollPatch.json"));

        AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
        AssertThat(
            sweetroll->GetData()->GetString("description").value(),
            Is().Not().Containing("sweet baked dessert")
        );
        AssertThat(
            sweetroll->GetData()->GetString("description").value(),
            Equals("A sweet roll is friggin' delicious")
        );
    });
}
