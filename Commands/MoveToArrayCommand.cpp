#include "MoveToArrayCommand.h"
#include "JsonParser.h"

MoveToArrayCommand::MoveToArrayCommand(const String& pathTo, const String& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(pathTo), _pathFrom(pathFrom) {}

MoveToArrayCommand::MoveToArrayCommand(String&& pathTo, String&& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(std::move(pathFrom)) {}

MoveToArrayCommand::MoveToArrayCommand(const String& pathTo, String&& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(pathTo), _pathFrom(std::move(pathFrom)) {}

MoveToArrayCommand::MoveToArrayCommand(String&& pathTo, const String& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(pathFrom) {}

void MoveToArrayCommand::execute() const {
    JsonParser::getInstance()->moveToArray(_pathTo.getData(), _pathFrom.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "Elements moved." << std::endl << std::endl;
    }
}
