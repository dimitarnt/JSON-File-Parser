#pragma once
#include "JsonCommand.h"

class SaveAsCommand : public JsonCommand {
private:
    String _fileName;
    String _path;

public:
    SaveAsCommand(const String& fileName, const String& path);
    SaveAsCommand(String&& fileName, String&& path);
    SaveAsCommand(const String& fileName, String&& path);
    SaveAsCommand(String&& fileName, const String& path);

    void execute() const override;
};
