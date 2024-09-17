#include "UndoAllCommand.h"
#include "JsonParser.h"

UndoAllCommand::UndoAllCommand() : JsonCommand(JsonCommandType::UNDO_ALL_COMMAND) {}

void UndoAllCommand::execute() const {
    JsonParser::getInstance()->undoAll();

    std::cout << "Actions undid." << std::endl << std::endl;
}
