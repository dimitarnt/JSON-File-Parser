#include "OpenCommand.h"
#include "String.h"
#include "JsonParser.h"

void OpenCommand::execute() const {
    String fileName;

    std::cout << "Enter file name:";
    std::cin >> fileName;

    try {
        JsonParser::getInstance()->openFile(fileName.getData());
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << "File opened." << std::endl << std::endl;
}
