#include "PrintCommand.h"
#include "JsonParser.h"

void PrintCommand::execute() const {

    try {
        JsonParser::getInstance()->print();
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << std::endl;
}
