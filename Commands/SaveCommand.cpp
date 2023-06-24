#include "SaveCommand.h"
#include "String.h"
#include "JsonParser.h"

void SaveCommand::execute() const {
    String answer;

    std::cout << "Would you like to save only a specific path?(yes/no)" << std::endl;
    std::cin >> answer;

    if(answer == "yes") {
        String path;

        std::cout << "Enter path:";
        std::cin >> path;

        try {
            JsonParser::getInstance()->save(path.getData());
        }
        catch(const std::exception& exception) {
            std::cout << exception.what() << std::endl << std::endl;
            return;
        }
    }
    else if(answer == "no") {

        try {
            JsonParser::getInstance()->save();
        }
        catch(const std::exception& exception) {
            std::cout << exception.what() << std::endl << std::endl;
            return;
        }
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }

    std::cout << "File saved." << std::endl << std::endl;
}
