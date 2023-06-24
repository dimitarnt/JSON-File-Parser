#include "CreateCommand.h"
#include "String.h"
#include "JsonParser.h"

void CreateCommand::execute() const {
    String path;
    String newStr;
    String answer;

    std::cout << "Would you like to create in an object or an array?(object/array)" << std::endl;
    std::cin >> answer;

    if(answer == "object") {
        String newKey;
        bool userIsAddressingStartingObject = false;

        if(JsonParser::getInstance()->getStartingNodeType() == JsonNodeType::JSON_OBJECT) {
            std::cout << "Would you like to create in the starting object?(yes/no)" << std::endl;
            std::cin >> answer;

            if(answer == "yes") {
                userIsAddressingStartingObject = true;
            }
            else if(answer != "no") {
                std::cout << "Invalid answer." << std::endl << std::endl;
                return;
            }
        }

        if(!userIsAddressingStartingObject) {
            std::cout << "Enter path:";
            std::cin >> path;
        }

        std::cout << "Enter new key:";
        std::cin >> newKey;

        std::cout << "Enter new string:";
        std::cin >> newStr;

        if(userIsAddressingStartingObject) {
            try{
                JsonParser::getInstance()->createInStartingObject(newKey.getData(), newStr.getData());
            }
            catch(const std::exception& exception) {
                std::cout << exception.what() << std::endl << std::endl;
                return;
            }
        }
        else {
            try{
                JsonParser::getInstance()->createInObject(path.getData(), newKey.getData(), newStr.getData());
            }
            catch(const std::exception& exception) {
                std::cout << exception.what() << std::endl << std::endl;
                return;
            }
        }

        std::cout << "New pair created." << std::endl << std::endl;
    }
    else if(answer == "array") {
        std::cout << "Enter path:";
        std::cin >> path;

        std::cout << "Enter new string:";
        std::cin >> newStr;

        try {
            JsonParser::getInstance()->createInArray(path.getData(), newStr.getData());
        }
        catch(const std::exception& exception) {
            std::cout << exception.what() << std::endl << std::endl;
            return;
        }

        std::cout << "New value created." << std::endl << std::endl;
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }
}
