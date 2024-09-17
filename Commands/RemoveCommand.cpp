#include "RemoveCommand.h"
#include "JsonParser.h"

RemoveCommand::RemoveCommand(const String& path) : JsonCommand(JsonCommandType::REMOVE_COMMAND), _path(path) {}

RemoveCommand::RemoveCommand(String&& path) : JsonCommand(JsonCommandType::REMOVE_COMMAND), _path(std::move(path)) {}

void RemoveCommand::execute() const {
    JsonParser::getInstance()->remove(_path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "Element removed." << std::endl << std::endl;
    }
}
