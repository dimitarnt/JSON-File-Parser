#include "RenameCommand.h"
#include "JsonParser.h"

RenameCommand::RenameCommand(const String& newKey, const String& path) : JsonCommand(JsonCommandType::RENAME_COMMAND),
                                                                         _newKey(newKey), _path(path) {}

RenameCommand::RenameCommand(String&& newKey, String&& path) : JsonCommand(JsonCommandType::RENAME_COMMAND),
                                                               _newKey(std::move(newKey)), _path(std::move(path)) {}

RenameCommand::RenameCommand(const String& newKey, String&& path) : JsonCommand(JsonCommandType::RENAME_COMMAND),
                                                                    _newKey(newKey), _path(std::move(path)) {}

RenameCommand::RenameCommand(String&& newKey, const String& path) : JsonCommand(JsonCommandType::RENAME_COMMAND),
                                                                    _newKey(std::move(newKey)), _path(path) {}

void RenameCommand::execute() const {
    JsonParser::getInstance()->rename(_newKey.getData(), _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New key set." << std::endl << std::endl;
    }
}
