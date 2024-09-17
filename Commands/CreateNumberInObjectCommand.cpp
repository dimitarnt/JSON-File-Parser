#include "CreateNumberInObjectCommand.h"
#include "JsonParser.h"

CreateNumberInObjectCommand::CreateNumberInObjectCommand(const String& newKey, const String& newNumber, const String& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(newKey), _newNumber(newNumber), _path(path) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(String&& newKey, String&& newNumber, String&& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newNumber(std::move(newNumber)),
    _path(std::move(path)) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(const String& newKey, const String& newNumber, String&& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(newKey), _newNumber(newNumber), _path(std::move(path)) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(const String& newKey, String&& newNumber, String&& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(newKey), _newNumber(std::move(newNumber)),
    _path(std::move(path)) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(String&& newKey, const String& newNumber, const String& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newNumber(newNumber), _path(path) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(String&& newKey, String&& newNumber, const String& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newNumber(std::move(newNumber)),
    _path(path) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(const String& newKey, String&& newNumber, const String& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(newKey), _newNumber(std::move(newNumber)), _path(path) {}

CreateNumberInObjectCommand::CreateNumberInObjectCommand(String&& newKey, const String& newNumber, String&& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_OBJECT_COMMAND), _newKey(std::move(newKey)), _newNumber(newNumber),
    _path(std::move(path)) {}

void CreateNumberInObjectCommand::execute() const {
    JsonParser::getInstance()->createNumberInObject(_newKey.getData(), _newNumber.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New pair created." << std::endl << std::endl;
    }
}
