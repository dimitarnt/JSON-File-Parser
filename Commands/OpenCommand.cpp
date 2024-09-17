#include "OpenCommand.h"
#include "JsonParser.h"

OpenCommand::OpenCommand(const String& fileName) : JsonCommand(JsonCommandType::OPEN_COMMAND), _fileName(fileName) {}

OpenCommand::OpenCommand(String&& fileName) : JsonCommand(JsonCommandType::OPEN_COMMAND), _fileName(std::move(fileName)) {}

void OpenCommand::execute() const {
    JsonParser::getInstance()->openFile(_fileName.getData());

    std::cout << "File opened." << std::endl << std::endl;
}
