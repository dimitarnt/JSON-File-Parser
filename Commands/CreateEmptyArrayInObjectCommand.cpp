#include "CreateEmptyArrayInObjectCommand.h"
#include "JsonParser.h"

CreateEmptyArrayInObjectCommand::CreateEmptyArrayInObjectCommand(const String& newKey, const String& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_ARRAY_IN_OBJECT_COMMAND), _newKey(newKey), _path(path) {}

CreateEmptyArrayInObjectCommand::CreateEmptyArrayInObjectCommand(String&& newKey, String&& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_ARRAY_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _path(std::move(path)) {}

CreateEmptyArrayInObjectCommand::CreateEmptyArrayInObjectCommand(const String& newKey, String&& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_ARRAY_IN_OBJECT_COMMAND), _newKey(newKey), _path(std::move(path)) {}

CreateEmptyArrayInObjectCommand::CreateEmptyArrayInObjectCommand(String&& newKey, const String& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_ARRAY_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _path(path) {}

void CreateEmptyArrayInObjectCommand::execute() const {
    JsonParser::getInstance()->createEmptyArrayInObject(_newKey.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New pair created." << std::endl << std::endl;
    }
}
