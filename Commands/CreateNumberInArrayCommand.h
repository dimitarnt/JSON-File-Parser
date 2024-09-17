#pragma once
#include "JsonCommand.h"

class CreateNumberInArrayCommand : public JsonCommand {
private:
    String _newNumber;
    String _path;

public:
    CreateNumberInArrayCommand(const String& newNumber, const String& path);
    CreateNumberInArrayCommand(String&& newNumber, String&& path);
    CreateNumberInArrayCommand(const String& newNumber, String&& path);
    CreateNumberInArrayCommand(String&& newNumber, const String& path);

    void execute() const override;
};
