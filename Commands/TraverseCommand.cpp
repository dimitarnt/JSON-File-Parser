#include "TraverseCommand.h"
#include "JsonParser.h"

TraverseCommand::TraverseCommand(const String& path) : JsonCommand(JsonCommandType::TRAVERSE_COMMAND), _path(path) {}

TraverseCommand::TraverseCommand(String&& path) : JsonCommand(JsonCommandType::TRAVERSE_COMMAND), _path(std::move(path)) {}

void TraverseCommand::execute() const {
    JsonParser::getInstance()->traverse(_path.getData());

    std::cout << std::endl << std::endl;
}
