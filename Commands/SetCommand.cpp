#include "SetCommand.h"
#include "String.h"
#include "JsonParser.h"

void SetCommand::execute() const {
    String path;
    String newStr;

    std::cout << "Enter set path:";
    std::cin >> path;

    std::cout << "Enter new string:";
    std::cin >> newStr;

    try {
        JsonParser::getInstance()->set(path.getData(), newStr.getData());
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << "New value set." << std::endl << std::endl;
}
