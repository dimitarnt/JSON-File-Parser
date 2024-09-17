#include "SetNumberCommand.h"
#include "JsonParser.h"

SetNumberCommand::SetNumberCommand(const String& newNumber, const String& path) : JsonCommand(JsonCommandType::SET_NUMBER_COMMAND),
                                                                                  _newNumber(newNumber), _path(path) {}

SetNumberCommand::SetNumberCommand(String&& newNumber, String&& path) : JsonCommand(JsonCommandType::SET_NUMBER_COMMAND),
                                                                        _newNumber(std::move(newNumber)), _path(std::move(path)) {}

SetNumberCommand::SetNumberCommand(const String& newNumber, String&& path) : JsonCommand(JsonCommandType::SET_NUMBER_COMMAND),
                                                                             _newNumber(newNumber), _path(std::move(path)) {}

SetNumberCommand::SetNumberCommand(String&& newNumber, const String& path) : JsonCommand(JsonCommandType::SET_NUMBER_COMMAND),
                                                                             _newNumber(std::move(newNumber)), _path(path) {}

void SetNumberCommand::execute() const {
    JsonParser::getInstance()->setNumber(_newNumber.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New number set." << std::endl << std::endl;
    }
}
