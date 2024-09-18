#include "HelpCommand.h"
#include <fstream>

HelpCommand::HelpCommand() : JsonCommand(JsonCommandType::HELP_COMMAND) {}

void HelpCommand::execute() const {
    std::ifstream in("helpInfo.txt");

    if(!in.is_open()) {
        throw std::ifstream::failure("Unable to open file");
    }

    std::cout << in.rdbuf();
}
