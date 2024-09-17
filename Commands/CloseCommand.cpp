#include "CloseCommand.h"
#include "JsonParser.h"

CloseCommand::CloseCommand() : JsonCommand(JsonCommandType::CLOSE_COMMAND) {}

void CloseCommand::execute() const {
    saveChangesPrompt();

    JsonParser::getInstance()->closeFile();

    std::cout << "File closed." << std::endl << std::endl;
}
