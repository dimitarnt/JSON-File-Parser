#include "UndoCommand.h"
#include "JsonParser.h"

UndoCommand::UndoCommand() : JsonCommand(JsonCommandType::UNDO_COMMAND) {}

void UndoCommand::execute() const {
    JsonParser::getInstance()->undo();

    std::cout << "Action undid." << std::endl << std::endl;
}
