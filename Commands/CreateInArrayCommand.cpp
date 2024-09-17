#include "CreateInArrayCommand.h"
#include "JsonParser.h"

CreateInArrayCommand::CreateInArrayCommand(const SharedPtr<JsonNode>& newNode, const String& path)
    : JsonCommand(JsonCommandType::CREATE_IN_ARRAY_COMMAND), _newNode(newNode), _path(path) {}

CreateInArrayCommand::CreateInArrayCommand(SharedPtr<JsonNode>&& newNode, String&& path)
    : JsonCommand(JsonCommandType::CREATE_IN_ARRAY_COMMAND), _newNode(std::move(newNode)), _path(std::move(path)) {}

CreateInArrayCommand::CreateInArrayCommand(const SharedPtr<JsonNode>& newNode, String&& path)
    : JsonCommand(JsonCommandType::CREATE_IN_ARRAY_COMMAND), _newNode(newNode), _path(std::move(path)) {}

CreateInArrayCommand::CreateInArrayCommand(SharedPtr<JsonNode>&& newNode, const String& path)
    : JsonCommand(JsonCommandType::CREATE_IN_ARRAY_COMMAND), _newNode(std::move(newNode)), _path(path) {}

void CreateInArrayCommand::execute() const {
    JsonParser::getInstance()->createInArray(_newNode, _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New element created." << std::endl << std::endl;
    }
}
