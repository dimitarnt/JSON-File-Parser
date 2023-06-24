#include "ValidateCommand.h"
#include "String.h"
#include "JsonParser.h"

void ValidateCommand::execute() const {
    String fileName;

    std::cout << "Enter file name:";
    std::cin >> fileName;

    try {
        JsonParser::validate(fileName.getData());
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << "File successfully validated." << std::endl << std::endl;
}
