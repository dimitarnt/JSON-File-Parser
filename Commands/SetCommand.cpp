#include "SetCommand.h"
#include "JsonParser.h"

SetCommand::SetCommand(const SharedPtr<JsonNode>& newNode, const String& path) : JsonCommand(JsonCommandType::SET_COMMAND),
                                                                                 _newNode(newNode), _path(path) {}

SetCommand::SetCommand(SharedPtr<JsonNode>&& newNode, String&& path) : JsonCommand(JsonCommandType::SET_COMMAND),
                                                                       _newNode(std::move(newNode)), _path(std::move(path)) {}

SetCommand::SetCommand(const SharedPtr<JsonNode>& newNode, String&& path) : JsonCommand(JsonCommandType::SET_COMMAND),
                                                                            _newNode(newNode), _path(std::move(path)) {}

SetCommand::SetCommand(SharedPtr<JsonNode>&& newNode, const String& path) : JsonCommand(JsonCommandType::SET_COMMAND),
                                                                            _newNode(std::move(newNode)), _path(path) {}

void SetCommand::execute() const {
    JsonParser::getInstance()->set(_newNode, _path.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "New element set." << std::endl << std::endl;
    }
}
