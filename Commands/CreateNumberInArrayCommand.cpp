#include "CreateNumberInArrayCommand.h"
#include "JsonParser.h"

CreateNumberInArrayCommand::CreateNumberInArrayCommand(const String& newNumber, const String& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_ARRAY_COMMAND), _newNumber(newNumber), _path(path) {}

CreateNumberInArrayCommand::CreateNumberInArrayCommand(String&& newNumber, String&& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_ARRAY_COMMAND), _newNumber(std::move(newNumber)), _path(std::move(path)) {}

CreateNumberInArrayCommand::CreateNumberInArrayCommand(const String& newNumber, String&& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_ARRAY_COMMAND), _newNumber(newNumber), _path(std::move(path)) {}

CreateNumberInArrayCommand::CreateNumberInArrayCommand(String&& newNumber, const String& path)
    : JsonCommand(JsonCommandType::CREATE_NUMBER_IN_ARRAY_COMMAND), _newNumber(std::move(newNumber)), _path(path) {}

void CreateNumberInArrayCommand::execute() const {
    JsonParser::getInstance()->createNumberInArray(_newNumber.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New number created." << std::endl << std::endl;
    }
}
