#pragma once
#include "JsonCommand.h"

class CreateStringInObjectCommand : public JsonCommand {
private:
    String _newKey;
    String _newStr;
    String _path;

public:
    CreateStringInObjectCommand(const String& newKey, const String& newStr, const String& path);
    CreateStringInObjectCommand(String&& newKey, String&& newStr, String&& path);
    CreateStringInObjectCommand(const String& newKey, const String& newStr, String&& path);
    CreateStringInObjectCommand(const String& newKey, String&& newStr, String&& path);
    CreateStringInObjectCommand(String&& newKey, const String& newStr, const String& path);
    CreateStringInObjectCommand(String&& newKey, String&& newStr, const String& path);
    CreateStringInObjectCommand(const String& newKey, String&& newStr, const String& path);
    CreateStringInObjectCommand(String&& newKey, const String& newStr, String&& path);

    void execute() const override;
};
