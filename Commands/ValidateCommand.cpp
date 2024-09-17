#include "ValidateCommand.h"
#include "JsonParser.h"

ValidateCommand::ValidateCommand(const String& fileName) : JsonCommand(JsonCommandType::VALIDATE_COMMAND), _fileName(fileName) {}

ValidateCommand::ValidateCommand(String&& fileName) : JsonCommand(JsonCommandType::VALIDATE_COMMAND), _fileName(std::move(fileName)) {}

void ValidateCommand::execute() const {
    JsonParser::validate(_fileName.getData());

    std::cout << "File successfully validated." << std::endl << std::endl;
}
