#include "CreateStringInObjectCommand.h"
#include "JsonParser.h"

CreateStringInObjectCommand::CreateStringInObjectCommand(const String& newKey, const String& newStr, const String& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(newKey), _newStr(newStr), _path(path) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(String&& newKey, String&& newStr, String&& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newStr(std::move(newStr)),
    _path(std::move(path)) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(const String& newKey, const String& newStr, String&& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(newKey), _newStr(newStr), _path(std::move(path)) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(const String& newKey, String&& newStr, String&& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(newKey), _newStr(std::move(newStr)), _path(std::move(path)) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(String&& newKey, const String& newStr, const String& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newStr(newStr), _path(path) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(String&& newKey, String&& newStr, const String& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newStr(std::move(newStr)), _path(path) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(const String& newKey, String&& newStr, const String& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(newKey), _newStr(std::move(newStr)), _path(path) {}

CreateStringInObjectCommand::CreateStringInObjectCommand(String&& newKey, const String& newStr, String&& path)
    : JsonCommand(JsonCommandType::CREATE_STRING_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newStr(newStr), _path(std::move(path)) {}

void CreateStringInObjectCommand::execute() const {
    JsonParser::getInstance()->createStringInObject(_newKey.getData(), _newStr.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New pair created." << std::endl << std::endl;
    }
}
