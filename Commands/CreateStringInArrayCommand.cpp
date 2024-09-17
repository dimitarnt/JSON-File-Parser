#include "CreateStringInArrayCommand.h"
#include "JsonParser.h"

CreateStringInArrayCommand::CreateStringInArrayCommand(const String& newStr, const String& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_ARRAY_COMMAND), _newStr(newStr), _path(path) {}

CreateStringInArrayCommand::CreateStringInArrayCommand(String&& newStr, String&& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_ARRAY_COMMAND), _newStr(std::move(newStr)), _path(std::move(path)) {}

CreateStringInArrayCommand::CreateStringInArrayCommand(const String& newStr, String&& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_ARRAY_COMMAND), _newStr(newStr), _path(std::move(path)) {}    
    
CreateStringInArrayCommand::CreateStringInArrayCommand(String&& newStr, const String& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_ARRAY_COMMAND), _newStr(std::move(newStr)), _path(path) {}    
    
void CreateStringInArrayCommand::execute() const {
    JsonParser::getInstance()->createStringInArray(_newStr.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New string created." << std::endl << std::endl;
    }
}    
    