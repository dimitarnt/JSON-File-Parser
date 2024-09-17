#pragma once
#include "JsonCommand.h"

class SetKeywordCommand : public JsonCommand {
private:
    String _newKeyword;
    String _path;

public:
    SetKeywordCommand(const String& newKeyword, const String& path);
    SetKeywordCommand(String&& newKeyword, String&& path);
    SetKeywordCommand(const String& newKeyword, String&& path);
    SetKeywordCommand(String&& newKeyword, const String& path);

    void execute() const override;
};
