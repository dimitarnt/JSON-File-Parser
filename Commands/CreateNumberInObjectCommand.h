#pragma once
#include "JsonCommand.h"

class CreateNumberInObjectCommand : public JsonCommand {
private:
    String _newKey;
    String _newNumber;
    String _path;

public:
    CreateNumberInObjectCommand(const String& newKey, const String& newNumber, const String& path);
    CreateNumberInObjectCommand(String&& newKey, String&& newNumber, String&& path);
    CreateNumberInObjectCommand(const String& newKey, const String& newNumber, String&& path);
    CreateNumberInObjectCommand(const String& newKey, String&& newNumber, String&& path);
    CreateNumberInObjectCommand(String&& newKey, const String& newNumber, const String& path);
    CreateNumberInObjectCommand(String&& newKey, String&& newNumber, const String& path);
    CreateNumberInObjectCommand(const String& newKey, String&& newNumber, const String& path);
    CreateNumberInObjectCommand(String&& newKey, const String& newNumber, String&& path);

    void execute() const override;
};
