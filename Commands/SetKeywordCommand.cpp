#include "SetKeywordCommand.h"
#include "JsonParser.h"

SetKeywordCommand::SetKeywordCommand(const String& newKeyword, const String& path) : JsonCommand(JsonCommandType::SET_KEYWORD_COMMAND),
                                                                                     _newKeyword(newKeyword), _path(path) {}

SetKeywordCommand::SetKeywordCommand(String&& newKeyword, String&& path) : JsonCommand(JsonCommandType::SET_KEYWORD_COMMAND),
                                                                           _newKeyword(std::move(newKeyword)), _path(std::move(path)) {}

SetKeywordCommand::SetKeywordCommand(const String& newKeyword, String&& path) : JsonCommand(JsonCommandType::SET_KEYWORD_COMMAND),
                                                                                _newKeyword(newKeyword), _path(std::move(path)) {}

SetKeywordCommand::SetKeywordCommand(String&& newKeyword, const String& path) : JsonCommand(JsonCommandType::SET_KEYWORD_COMMAND),
                                                                                _newKeyword(std::move(newKeyword)), _path(path) {}

void SetKeywordCommand::execute() const {
    JsonParser::getInstance()->setKeyword(_newKeyword.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New keyword set." << std::endl << std::endl;
    }
}
