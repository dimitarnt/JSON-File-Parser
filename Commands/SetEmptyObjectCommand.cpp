#include "SetEmptyObjectCommand.h"
#include "JsonParser.h"

SetEmptyObjectCommand::SetEmptyObjectCommand(const String& path) : JsonCommand(JsonCommandType::SET_EMPTY_OBJECT_COMMAND),
                                                                   _path(path) {}

SetEmptyObjectCommand::SetEmptyObjectCommand(String&& path) : JsonCommand(JsonCommandType::SET_EMPTY_OBJECT_COMMAND),
                                                              _path(std::move(path)) {}

void SetEmptyObjectCommand::execute() const {
    JsonParser::getInstance()->setEmptyObject(_path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New object set." << std::endl << std::endl;
    }
}
