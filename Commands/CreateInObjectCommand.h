#pragma once
#include "JsonCommand.h"
#include "JsonPair.h"

class CreateInObjectCommand : public JsonCommand {
private:
    JsonPair _newPair;
    String _path;

public:
    CreateInObjectCommand(const JsonPair& newPair, const String& path);
    CreateInObjectCommand(JsonPair&& newPair, String&& path);
    CreateInObjectCommand(const JsonPair& newPair, String&& path);
    CreateInObjectCommand(JsonPair&& newPair, const String& path);

    void execute() const override;
};
