#include <_Log_.h>
_LogToFile_("Editor.log");
//

#include "Camps1te/Editor/Main.h"

int main(int argc, char* argv[]) { return Camps1te::Editor::Main(argc, argv); }

// Definitions

namespace Camps1te::Editor::Paths::Development {
    std::filesystem::path ProjectFolder{"../../../../"};
    std::filesystem::path ResourcesFolder{ProjectFolder / "Resources"};
    std::filesystem::path DataFolder{ResourcesFolder / "Data"};
    std::filesystem::path QssFile{ResourcesFolder / "Editor" / "Editor.qss"};
}
