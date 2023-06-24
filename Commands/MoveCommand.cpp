#include "MoveCommand.h"
#include "String.h"
#include "JsonParser.h"

void MoveCommand::execute() const {
    String pathFrom;
    String pathTo;
    String answer;

    std::cout << "Would you like to move in an object or an array?(object/array)" << std::endl;
    std::cin >> answer;

    std::cout << "Enter path to move from:";
    std::cin >> pathFrom;

    if(answer == "object") {
        bool userIsAddressingStartingObject = false;

        if(JsonParser::getInstance()->getStartingNodeType() == JsonNodeType::JSON_OBJECT) {
            std::cout << "Would you like to move to the starting object?(yes/no)" << std::endl;
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
            std::cout << "Enter path to move to:";
            std::cin >> pathTo;
        }

        if(userIsAddressingStartingObject) {
            try {
                JsonParser::getInstance()->moveToStartingObject(pathFrom.getData());
            }
            catch(const std::exception& exception) {
                std::cout << exception.what() << std::endl << std::endl;
                return;
            }
        }
        else {
            try {
                JsonParser::getInstance()->moveToObject(pathFrom.getData(), pathTo.getData());
            }
            catch(const std::exception& exception) {
                std::cout << exception.what() << std::endl << std::endl;
                return;
            }
        }

        std::cout << "Elements moved." << std::endl << std::endl;
    }
    else if(answer == "array") {
        std::cout << "Enter path to move to:";
        std::cin >> pathTo;

        try {
            JsonParser::getInstance()->moveToArray(pathFrom.getData(), pathTo.getData());
        }
        catch(const std::exception& exception) {
            std::cout << exception.what() << std::endl << std::endl;
            return;
        }

        std::cout << "Elements moved." << std::endl << std::endl;
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }
}
