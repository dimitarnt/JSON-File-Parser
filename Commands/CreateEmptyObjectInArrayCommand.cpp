#include "CreateEmptyObjectInArrayCommand.h"
#include "JsonParser.h"

CreateEmptyObjectInArrayCommand::CreateEmptyObjectInArrayCommand(const String& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_OBJECT_IN_ARRAY_COMMAND), _path(path) {}

CreateEmptyObjectInArrayCommand::CreateEmptyObjectInArrayCommand(String&& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_OBJECT_IN_ARRAY_COMMAND), _path(std::move(path)) {}

void CreateEmptyObjectInArrayCommand::execute() const {
    JsonParser::getInstance()->createEmptyObjectInArray(_path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New object created." << std::endl << std::endl;
    }
}
