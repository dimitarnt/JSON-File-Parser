#include "CreateKeywordInObjectCommand.h"
#include "JsonParser.h"

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(const String& newKey, const String& newKeyword, const String& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(newKey), _newKeyword(newKeyword), _path(path) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(String&& newKey, String&& newKeyword, String&& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newKeyword(std::move(newKeyword)),
    _path(std::move(path)) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(const String& newKey, const String& newKeyword, String&& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(newKey), _newKeyword(newKeyword), _path(std::move(path)) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(const String& newKey, String&& newKeyword, String&& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(newKey), _newKeyword(std::move(newKeyword)),
    _path(std::move(path)) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(String&& newKey, const String& newKeyword, const String& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newKeyword(newKeyword), _path(path) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(String&& newKey, String&& newKeyword, const String& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newKeyword(std::move(newKeyword)),
    _path(path) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(const String& newKey, String&& newKeyword, const String& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(newKey), _newKeyword(std::move(newKeyword)), _path(path) {}

CreateKeywordInObjectCommand::CreateKeywordInObjectCommand(String&& newKey, const String& newKeyword, String&& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newKeyword(newKeyword),
    _path(std::move(path)) {}

void CreateKeywordInObjectCommand::execute() const {
    JsonParser::getInstance()->createKeywordInObject(_newKey.getData(), _newKeyword.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New pair created." << std::endl << std::endl;
    }
}
