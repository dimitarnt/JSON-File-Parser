#include "SetEmptyArrayCommand.h"
#include "JsonParser.h"

SetEmptyArrayCommand::SetEmptyArrayCommand(const String& path) : JsonCommand(JsonCommandType::SET_EMPTY_ARRAY_COMMAND),
                                                                 _path(path) {}

SetEmptyArrayCommand::SetEmptyArrayCommand(String&& path) : JsonCommand(JsonCommandType::SET_EMPTY_ARRAY_COMMAND),
                                                            _path(std::move(path)) {}

void SetEmptyArrayCommand::execute() const {
    JsonParser::getInstance()->setEmptyArray(_path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New array set." << std::endl << std::endl;
    }
}
