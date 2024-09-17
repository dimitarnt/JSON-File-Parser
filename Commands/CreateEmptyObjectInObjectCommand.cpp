#include "CreateEmptyObjectInObjectCommand.h"
#include "JsonParser.h"

CreateEmptyObjectInObjectCommand::CreateEmptyObjectInObjectCommand(const String& newKey, const String& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_OBJECT_IN_OBJECT_COMMAND), _newKey(newKey), _path(path) {}

CreateEmptyObjectInObjectCommand::CreateEmptyObjectInObjectCommand(String&& newKey, String&& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_OBJECT_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _path(std::move(path)) {}

CreateEmptyObjectInObjectCommand::CreateEmptyObjectInObjectCommand(const String& newKey, String&& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_OBJECT_IN_OBJECT_COMMAND), _newKey(newKey), _path(std::move(path)) {}

CreateEmptyObjectInObjectCommand::CreateEmptyObjectInObjectCommand(String&& newKey, const String& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_OBJECT_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _path(path) {}

void CreateEmptyObjectInObjectCommand::execute() const {
    JsonParser::getInstance()->createEmptyObjectInObject(_newKey.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New pair created." << std::endl << std::endl;
    }
}
