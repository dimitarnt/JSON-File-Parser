#pragma once
#include "JsonCommand.h"

class CreateKeywordInArrayCommand : public JsonCommand {
private:
    String _newKeyword;
    String _path;

public:
    CreateKeywordInArrayCommand(const String& newKeyword, const String& path);
    CreateKeywordInArrayCommand(String&& newKeyword, String&& path);
    CreateKeywordInArrayCommand(const String& newKeyword, String&& path);
    CreateKeywordInArrayCommand(String&& newKeyword, const String& path);

    void execute() const override;
};
