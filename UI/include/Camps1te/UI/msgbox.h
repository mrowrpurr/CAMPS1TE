#pragma once

#define msgbox(...) QMessageBox::information(nullptr, "", string_format(__VA_ARGS__).c_str())
