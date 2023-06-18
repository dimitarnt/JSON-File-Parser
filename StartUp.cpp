#include "StartUp.h"
#include "JsonParser.h"

void StartUp::run() {
    String command;

    std::cout << "Welcome to JsonFileParser v1.0!" << std::endl << std::endl;

    while(true) {
        std::cin >> command;

        if(command == "open") {
            runOpenCommand();
        }
        else if(command == "close") {
            runCloseCommand();
        }
        else if(command == "validate") {
            runValidateCommand();
        }
        else if(command == "print") {
            runPrintCommand();
        }
        else if(command == "search") {
            runSearchCommand();
        }
        else if(command == "set") {
            runSetCommand();
        }
        else if(command == "create") {
            runCreateCommand();
        }
        else if(command == "remove") {
            runRemoveCommand();
        }
        else if(command == "move") {
            runMoveCommand();
        }
        else if(command == "save") {
            runSaveCommand();
        }
        else if(command == "save_as") {
            runSaveAsCommand();
        }
        else if(command == "help") {
            runHelpCommand();
        }
        else if(command == "exit") {
            runExitCommand();
            return;
        }
        else {
            std::cout << "Unknown command." << std::endl << std::endl;
        }
    }
}

void StartUp::runOpenCommand() {
    String fileName;

    std::cout << "Enter file name:";
    std::cin >> fileName;

    JsonParser::getInstance()->openFile(fileName.getData());

    std::cout << "File opened." << std::endl << std::endl;
}

void StartUp::runCloseCommand() {
    JsonParser::getInstance()->closeFile();

    std::cout << "File closed." << std::endl << std::endl;
}

void StartUp::runValidateCommand() {
    String fileName;

    std::cout << "Enter file name:";
    std::cin >> fileName;

    JsonParser::validate(fileName.getData());

    std::cout << "File successfully validated." << std::endl << std::endl;
}

void StartUp::runPrintCommand() {
    JsonParser::getInstance()->print();

    std::cout << std::endl;
}

void StartUp::runSearchCommand() {
    String searchTerm;

    std::cout << "Enter search term:";
    std::cin >> searchTerm;

    JsonParser::getInstance()->search(searchTerm.getData());

    std::cout << std::endl;
}

void StartUp::runSetCommand() {
    String path;
    String newStr;

    std::cout << "Enter set path:";
    std::cin >> path;

    std::cout << "Enter new string:";
    std::cin >> newStr;

    JsonParser::getInstance()->set(path.getData(), newStr.getData());

    std::cout << "New value set." << std::endl << std::endl;
}

void StartUp::runCreateCommand() {
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

        userIsAddressingStartingObject ? JsonParser::getInstance()->createInStartingObject(newKey.getData(), newStr.getData()) :
        JsonParser::getInstance()->createInObject(path.getData(), newKey.getData(), newStr.getData());

        std::cout << "New pair created." << std::endl << std::endl;
    }
    else if(answer == "array") {
        std::cout << "Enter path:";
        std::cin >> path;

        std::cout << "Enter new string:";
        std::cin >> newStr;

        JsonParser::getInstance()->createInArray(path.getData(), newStr.getData());

        std::cout << "New value created." << std::endl << std::endl;
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }
}

void StartUp::runRemoveCommand() {
    String path;

    std::cout << "Enter path:";
    std::cin >> path;

    JsonParser::getInstance()->remove(path.getData());

    std::cout << "Element removed." << std::endl << std::endl;
}

void StartUp::runMoveCommand() {
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

        userIsAddressingStartingObject ? JsonParser::getInstance()->moveToStartingObject(pathFrom.getData()) :
        JsonParser::getInstance()->moveToObject(pathFrom.getData(), pathTo.getData());

        std::cout << "Elements moved." << std::endl << std::endl;
    }
    else if(answer == "array") {
        std::cout << "Enter path to move to:";
        std::cin >> pathTo;

        JsonParser::getInstance()->moveToArray(pathFrom.getData(), pathTo.getData());

        std::cout << "Elements moved." << std::endl << std::endl;
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }
}

void StartUp::runSaveCommand() {
    String answer;

    std::cout << "Would you like to save only a specific path?(yes/no)" << std::endl;
    std::cin >> answer;

    if(answer == "yes") {
        String path;

        std::cout << "Enter path:";
        std::cin >> path;

        JsonParser::getInstance()->save(path.getData());
    }
    else if(answer == "no") {
        JsonParser::getInstance()->save();
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }

    std::cout << "File saved." << std::endl << std::endl;
}

void StartUp::runSaveAsCommand() {
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

        JsonParser::getInstance()->saveAs(fileName.getData(), path.getData());
    }
    else if(answer == "no") {
        JsonParser::getInstance()->saveAs(fileName.getData());
    }
    else {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }

    std::cout << "File saved." << std::endl << std::endl;
}

void StartUp::runHelpCommand() {
    std::cout << "List of available commands:" << std::endl;

    std::cout << "1)open (Open json file)" << std::endl;
    std::cout << "2)close (Close json file)" << std::endl;
    std::cout << "3)validate (Validate json file)" << std::endl;
    std::cout << "4)print (Print opened json file)" << std::endl;
    std::cout << "5)search (Search in the opened json file by key)" << std::endl;
    std::cout << "6)set (Set new string value by path)" << std::endl;
    std::cout << "7)create (Create new json pair or an element in an array)" << std::endl;
    std::cout << "8)remove (Remove a json pair or an element in an array)" << std::endl;
    std::cout << "9)move (Move a json pair or an element in an array)" << std::endl;
    std::cout << "10)save (Save the whole current file or just a given path)" << std::endl;
    std::cout << "10)save_as (Save the whole current file or just a given path in a different file)" << std::endl;

    std::cout << std::endl;
}

void StartUp::runExitCommand() {
    String answer;

    std::cout << "Would you like to save changes?(yes/no)" << std::endl;
    std::cin >> answer;

    if(answer == "yes") {
        JsonParser::getInstance()->save();
    }
    else if(answer != "no") {
        std::cout << "Invalid answer." << std::endl << std::endl;
        return;
    }

    JsonParser::freeInstance();
}
