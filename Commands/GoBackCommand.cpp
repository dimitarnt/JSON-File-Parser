#include "GoBackCommand.h"
#include "JsonParser.h"

GoBackCommand::GoBackCommand() : JsonCommand(JsonCommandType::GO_BACK_COMMAND) {}

void GoBackCommand::execute() const {
    JsonParser::getInstance()->goBack();

    std::cout << std::endl << std::endl;
}
