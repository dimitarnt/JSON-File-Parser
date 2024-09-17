#pragma once
#include "JsonCommand.h"

class SetNumberCommand : public JsonCommand {
private:
    String _newNumber;
    String _path;

public:
    SetNumberCommand(const String& newNumber, const String& path);
    SetNumberCommand(String&& newNumber, String&& path);
    SetNumberCommand(const String& newNumber, String&& path);
    SetNumberCommand(String&& newNumber, const String& path);

    void execute() const override;
};
