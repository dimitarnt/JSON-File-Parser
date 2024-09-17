#pragma once
#include "JsonCommand.h"

class CreateKeywordInObjectCommand : public JsonCommand {
private:
    String _newKey;
    String _newKeyword;
    String _path;

public:
    CreateKeywordInObjectCommand(const String& newKey, const String& newKeyword, const String& path);
    CreateKeywordInObjectCommand(String&& newKey, String&& newKeyword, String&& path);
    CreateKeywordInObjectCommand(const String& newKey, const String& newKeyword, String&& path);
    CreateKeywordInObjectCommand(const String& newKey, String&& newKeyword, String&& path);
    CreateKeywordInObjectCommand(String&& newKey, const String& newKeyword, const String& path);
    CreateKeywordInObjectCommand(String&& newKey, String&& newKeyword, const String& path);
    CreateKeywordInObjectCommand(const String& newKey, String&& newKeyword, const String& path);
    CreateKeywordInObjectCommand(String&& newKey, const String& newKeyword, String&& path);

    void execute() const override;
};
