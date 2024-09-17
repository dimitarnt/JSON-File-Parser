#include "CreateInObjectCommand.h"
#include "JsonParser.h"

CreateInObjectCommand::CreateInObjectCommand(const JsonPair& newPair, const String& path)
    : JsonCommand(JsonCommandType::CREATE_IN_OBJECT_COMMAND), _newPair(newPair), _path(path) {}

CreateInObjectCommand::CreateInObjectCommand(JsonPair&& newPair, String&& path)
    : JsonCommand(JsonCommandType::CREATE_IN_OBJECT_COMMAND), _newPair(std::move(newPair)), _path(std::move(path)) {}

CreateInObjectCommand::CreateInObjectCommand(const JsonPair& newPair, String&& path)
    : JsonCommand(JsonCommandType::CREATE_IN_OBJECT_COMMAND), _newPair(newPair), _path(std::move(path)) {}

CreateInObjectCommand::CreateInObjectCommand(JsonPair&& newPair, const String& path)
    : JsonCommand(JsonCommandType::CREATE_IN_OBJECT_COMMAND), _newPair(std::move(newPair)), _path(path) {}

void CreateInObjectCommand::execute() const {
    JsonParser::getInstance()->createInObject(_newPair, _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New pair created." << std::endl << std::endl;
    }
}
