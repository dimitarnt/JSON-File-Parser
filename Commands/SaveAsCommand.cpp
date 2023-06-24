#include "SaveAsCommand.h"
#include "String.h"
#include "JsonParser.h"

void SaveAsCommand::execute() const {
    String fileName;
    String answer;

    std::cout << "Enter file name to save to:";
    std::cin >> fileName;

    std::cout << "Would you like to save only a specific path?(yes/no)" << std::endl;
    std::cin >> answer;

    if(answer == "yes") {
        String path;

        std::cout << "Enter path:";
        std::cin >> path;

        try {
            JsonParser::getInstance()->saveAs(fileName.getData(), path.getData());
        }
        catch(const std::exception& exception) {
            std::cout << exception.what() << std::endl << std::endl;
            return;
        }
    }
    else if(answer == "no") {

        try {
            JsonParser::getInstance()->saveAs(fileName.getData());
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
