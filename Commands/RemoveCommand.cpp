#include "RemoveCommand.h"
#include "String.h"
#include "JsonParser.h"

void RemoveCommand::execute() const {
    String path;

    std::cout << "Enter path:";
    std::cin >> path;

    try {
        JsonParser::getInstance()->remove(path.getData());
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << "Element removed." << std::endl << std::endl;
}
