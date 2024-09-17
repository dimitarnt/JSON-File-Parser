#include "PrintCommand.h"
#include "JsonParser.h"

PrintCommand::PrintCommand() : JsonCommand(JsonCommandType::PRINT_COMMAND) {}

void PrintCommand::execute() const {
    JsonParser::getInstance()->print();

    std::cout << std::endl << std::endl;
}
