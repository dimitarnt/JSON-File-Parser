#include "CreateKeywordInArrayCommand.h"
#include "JsonParser.h"

CreateKeywordInArrayCommand::CreateKeywordInArrayCommand(const String& newKeyword, const String& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_ARRAY_COMMAND), _newKeyword(newKeyword), _path(path) {}

CreateKeywordInArrayCommand::CreateKeywordInArrayCommand(String&& newKeyword, String&& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_ARRAY_COMMAND), _newKeyword(std::move(newKeyword)), _path(std::move(path)) {}

CreateKeywordInArrayCommand::CreateKeywordInArrayCommand(const String& newKeyword, String&& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_ARRAY_COMMAND), _newKeyword(newKeyword), _path(std::move(path)) {}

CreateKeywordInArrayCommand::CreateKeywordInArrayCommand(String&& newKeyword, const String& path)
    : JsonCommand(JsonCommandType::CREATE_KEYWORD_IN_ARRAY_COMMAND), _newKeyword(std::move(newKeyword)), _path(path) {}

void CreateKeywordInArrayCommand::execute() const {
    JsonParser::getInstance()->createKeywordInArray(_newKeyword.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New keyword created." << std::endl << std::endl;
    }
}
