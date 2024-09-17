#include "PreviousCommand.h"
#include "JsonParser.h"

PreviousCommand::PreviousCommand() : JsonCommand(JsonCommandType::PREVIOUS_COMMAND) {}

void PreviousCommand::execute() const {
    JsonParser::getInstance()->previous();

    std::cout << std::endl << std::endl;
}
