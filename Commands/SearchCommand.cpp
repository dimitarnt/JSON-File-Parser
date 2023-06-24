#include "SearchCommand.h"
#include "String.h"
#include "JsonParser.h"

void SearchCommand::execute() const {
    String searchTerm;

    std::cout << "Enter search term:";
    std::cin >> searchTerm;

    try {
        JsonParser::getInstance()->search(searchTerm.getData());
    }
    catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl << std::endl;
        return;
    }

    std::cout << std::endl;
}
