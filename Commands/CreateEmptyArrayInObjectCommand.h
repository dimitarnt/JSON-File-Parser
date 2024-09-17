#pragma once
#include "JsonCommand.h"

class CreateEmptyArrayInObjectCommand : public JsonCommand {
private:
    String _newKey;
    String _path;

public:
    CreateEmptyArrayInObjectCommand(const String& newKey, const String& path);
    CreateEmptyArrayInObjectCommand(String&& newKey, String&& path);
    CreateEmptyArrayInObjectCommand(const String& newKey, String&& path);
    CreateEmptyArrayInObjectCommand(String&& newKey, const String& path);

    void execute() const override;
};
