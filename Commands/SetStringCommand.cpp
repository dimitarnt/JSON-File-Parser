#include "SetStringCommand.h"
#include "JsonParser.h"

SetStringCommand::SetStringCommand(const String& newStr, const String& path) : JsonCommand(JsonCommandType::SET_STRING_COMMAND),
                                                                               _newStr(newStr), _path(path) {}

SetStringCommand::SetStringCommand(String&& newStr, String&& path) : JsonCommand(JsonCommandType::SET_STRING_COMMAND),
                                                                     _newStr(std::move(newStr)), _path(std::move(path)) {}

SetStringCommand::SetStringCommand(const String& newStr, String&& path) : JsonCommand(JsonCommandType::SET_STRING_COMMAND),
                                                                          _newStr(newStr), _path(std::move(path)) {}

SetStringCommand::SetStringCommand(String&& newStr, const String& path) : JsonCommand(JsonCommandType::SET_STRING_COMMAND),
                                                                          _newStr(std::move(newStr)), _path(path) {}

void SetStringCommand::execute() const {
    JsonParser::getInstance()->setString(_newStr.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New string set." << std::endl << std::endl;
    }
}
