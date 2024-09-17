#pragma once
#include "JsonCommand.h"

class SetStringCommand : public JsonCommand {
private:
    String _newStr;
    String _path;

public:
    SetStringCommand(const String& newStr, const String& path);
    SetStringCommand(String&& newStr, String&& path);
    SetStringCommand(const String& newStr, String&& path);
    SetStringCommand(String&& newStr, const String& path);

    void execute() const override;
};
