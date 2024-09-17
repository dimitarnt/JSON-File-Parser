#include "MoveFromCommand.h"
#include "JsonParser.h"

MoveFromCommand::MoveFromCommand(const String& pathFrom) : JsonCommand(JsonCommandType::MOVE_FROM_COMMAND), _pathFrom(pathFrom) {}

MoveFromCommand::MoveFromCommand(String&& pathFrom) : JsonCommand(JsonCommandType::MOVE_FROM_COMMAND), _pathFrom(std::move(pathFrom)) {}

void MoveFromCommand::execute() const {
    JsonParser::getInstance()->moveFrom(_pathFrom.getData());

    std::cout << std::endl << std::endl;
}
