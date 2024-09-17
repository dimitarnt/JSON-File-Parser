#pragma once
#include "JsonCommand.h"

class CreateEmptyObjectInObjectCommand : public JsonCommand {
private:
    String _newKey;
    String _path;

public:
    CreateEmptyObjectInObjectCommand(const String& newKey, const String& path);
    CreateEmptyObjectInObjectCommand(String&& newKey, String&& path);
    CreateEmptyObjectInObjectCommand(const String& newKey, String&& path);
    CreateEmptyObjectInObjectCommand(String&& newKey, const String& path);

    void execute() const override;
};
