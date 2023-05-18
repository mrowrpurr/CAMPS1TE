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

    // it("records are inserted as deep copies", [](){});
}

//     // TODO : JsonDataFile in own file plz

//     it("data files have identifiers", []() {
//         auto dataFile1 =
//             std::unique_ptr<DataFile>(new JsonDataFile{GetResourcePath("FoodDataFile.json")});
//         auto dataFile2 =
//             std::unique_ptr<DataFile>(new
//             JsonDataFile{GetResourcePath("FoodSweetrollPatch.json")});

//         AssertThat(dataFile1->GetIdentifier(), Equals("FoodDataFile"));
//         AssertThat(dataFile2->GetIdentifier(), Equals("FoodSweetrollPatch"));
//     });

//     it("can load and read *something* from a data file", []() {
//         auto jsonFilePath = GetResourcePath("FoodDataFile.json");
//         auto dataFile     = std::unique_ptr<DataFile>(new JsonDataFile{jsonFilePath});

//         // ASSERT that we can read some stuff!
//         auto sweetroll = dataFile->GetRecord("sweetroll");
//         AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
//         AssertThat(
//             sweetroll->GetData()->GetString("description").value(), Contains("sweet baked
//             dessert")
//         );
//     });

//     // it("can read *all* records from a data file", []() {
//     //     auto jsonFilePath = GetResourcePath("FoodDataFile.json");
//     //     auto dataFile     = std::unique_ptr<DataFile>(new JsonDataFile{jsonFilePath});

//     //     auto records = dataFile->GetAllRecords();
//     //     AssertThat(records.size(), Equals(2));
//     // });

//     // TODO : datastore in own file plz

//     // it("can load and read data files into a data store", []() {
//     //     auto jsonFilePath = GetResourcePath("FoodDataFile.json");
//     //     auto dataFile     = std::unique_ptr<DataFile>(new JsonDataFile{jsonFilePath});

//     //     // Data source!
//     //     auto dataStore = std::unique_ptr<DataStore>(new JsonDataStore{});

//     //     // Add the data file to the data store
//     //     dataFile.AddToDataStore(dataStore);

//     //     // ASSERT that we can read some stuff!
//     //     auto sweetroll = dataStore->GetRecord("FoodDataFile", "sweetroll");
//     //     AssertThat(sweetroll->GetData()->GetString("name").value(), Equals("Sweetroll"));
//     //     AssertThat(
//     //         sweetroll->GetData()->GetString("description").value(), Contains("sweet baked
//     //         dessert")
//     //     );
//     // });

//     // it("loading a datafile can overwrite fields from a previously loaded data file", []() {
//     //     auto dataFile1 =
//     //         std::unique_ptr<DataFile>(new JsonDataFile{GetResourcePath("FoodDataFile.json")});

//     //     auto dataFile2 =
//     //         std::unique_ptr<DataFile>(new
//     //         JsonDataFile{GetResourcePath("FoodSweetrollPatch.json")});

//     //     //
//     // });
// }
