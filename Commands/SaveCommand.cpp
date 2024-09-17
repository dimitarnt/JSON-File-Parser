#include "SaveCommand.h"
#include "JsonParser.h"

SaveCommand::SaveCommand(const String& path) : JsonCommand(JsonCommandType::SAVE_COMMAND), _path(path) {}

SaveCommand::SaveCommand(String&& path) : JsonCommand(JsonCommandType::SAVE_COMMAND), _path(std::move(path)) {}

void SaveCommand::execute() const {
    JsonParser::getInstance()->save(_path.getData());

    std::cout << "File saved." << std::endl << std::endl;
}
