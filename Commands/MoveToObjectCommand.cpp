#include "MoveToObjectCommand.h"
#include "JsonParser.h"

MoveToObjectCommand::MoveToObjectCommand(const String& pathTo, const String& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_OBJECT_COMMAND), _pathTo(pathTo), _pathFrom(pathFrom) {}

MoveToObjectCommand::MoveToObjectCommand(String&& pathTo, String&& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_OBJECT_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(std::move(pathFrom)) {}

MoveToObjectCommand::MoveToObjectCommand(const String& pathTo, String&& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_OBJECT_COMMAND), _pathTo(pathTo), _pathFrom(std::move(pathFrom)) {}

MoveToObjectCommand::MoveToObjectCommand(String&& pathTo, const String& pathFrom)
    : JsonCommand(JsonCommandType::MOVE_TO_OBJECT_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(pathFrom) {}

MoveToObjectCommand::MoveToObjectCommand(const String& pathTo, const String& pathFrom, const String& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(pathTo), _pathFrom(pathFrom), _oldKey(oldKey) {}

MoveToObjectCommand::MoveToObjectCommand(const String& pathTo, const String& pathFrom, String&& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(pathTo), _pathFrom(pathFrom), _oldKey(std::move(oldKey)) {}

MoveToObjectCommand::MoveToObjectCommand(const String& pathTo, String&& pathFrom, String&& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(pathTo), _pathFrom(std::move(pathFrom)),
    _oldKey(std::move(oldKey)) {}

MoveToObjectCommand::MoveToObjectCommand(String&& pathTo, const String& pathFrom, String&& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(pathFrom),
    _oldKey(std::move(oldKey)) {}

MoveToObjectCommand::MoveToObjectCommand(String&& pathTo, String&& pathFrom, const String& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(std::move(pathFrom)),
    _oldKey(oldKey) {}

MoveToObjectCommand::MoveToObjectCommand(String&& pathTo, const String& pathFrom, const String& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(pathFrom), _oldKey(oldKey) {}

MoveToObjectCommand::MoveToObjectCommand(const String& pathTo, String&& pathFrom, const String& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(pathTo), _pathFrom(std::move(pathFrom)), _oldKey(oldKey) {}

MoveToObjectCommand::MoveToObjectCommand(String&& pathTo, String&& pathFrom, String&& oldKey)
    : JsonCommand(JsonCommandType::MOVE_TO_ARRAY_COMMAND), _pathTo(std::move(pathTo)), _pathFrom(std::move(pathFrom)),
    _oldKey(std::move(oldKey)) {}

void MoveToObjectCommand::execute() const {
    JsonParser::getInstance()->moveToObject(_pathTo.getData(), _pathFrom.getData(), _oldKey.getData());

    if(!JsonParser::getInstance()->actionIsBeingUndone()) {
        std::cout << "Elements moved." << std::endl << std::endl;
    }

}