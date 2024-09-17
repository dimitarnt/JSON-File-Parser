#include "ExitCommand.h"
#include "JsonParser.h"

ExitCommand::ExitCommand() : JsonCommand(JsonCommandType::EXIT_COMMAND) {}

void ExitCommand::execute() const {
    saveChangesPrompt();

    JsonParser::freeInstance();
}
