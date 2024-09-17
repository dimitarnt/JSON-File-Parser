#include "CreateEmptyArrayInArrayCommand.h"
#include "JsonParser.h"

CreateEmptyArrayInArrayCommand::CreateEmptyArrayInArrayCommand(const String& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_ARRAY_IN_ARRAY_COMMAND), _path(path) {}

CreateEmptyArrayInArrayCommand::CreateEmptyArrayInArrayCommand(String&& path)
    : JsonCommand(JsonCommandType::CREATE_EMPTY_ARRAY_IN_ARRAY_COMMAND), _path(std::move(path)) {}

void CreateEmptyArrayInArrayCommand::execute() const {
    JsonParser::getInstance()->createEmptyArrayInArray(_path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New array created." << std::endl << std::endl;
    }
}
