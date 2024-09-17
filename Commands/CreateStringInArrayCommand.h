#pragma once
#include "JsonCommand.h"

class CreateStringInArrayCommand : public JsonCommand {
private:
    String _newStr;
    String _path;

public:
    CreateStringInArrayCommand(const String& newStr, const String& path);
    CreateStringInArrayCommand(String&& newStr, String&& path);
    CreateStringInArrayCommand(const String& newStr, String&& path);
    CreateStringInArrayCommand(String&& newStr, const String& path);

    void execute() const override;
};
