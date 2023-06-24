#include "ExitCommand.h"
#include "String.h"
#include "JsonParser.h"

void ExitCommand::execute() const {
    String answer;

    saveChangesPrompt();

    try {
        JsonParser::freeInstance();
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }
}
