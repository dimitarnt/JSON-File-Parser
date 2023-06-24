#include "CloseCommand.h"
#include "JsonParser.h"

void CloseCommand::execute() const {
    saveChangesPrompt();

    try {
        JsonParser::getInstance()->closeFile();
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << "File closed." << std::endl << std::endl;
}
