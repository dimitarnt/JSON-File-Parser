#include "StartUp.h"
#include "String.h"
#include "OpenCommand.h"
#include "CloseCommand.h"
#include "ValidateCommand.h"
#include "PrintCommand.h"
#include "SearchCommand.h"
#include "SetCommand.h"
#include "CreateCommand.h"
#include "RemoveCommand.h"
#include "MoveCommand.h"
#include "SaveCommand.h"
#include "SaveAsCommand.h"
#include "HelpCommand.h"
#include "ExitCommand.h"

void StartUp::run() {
    String command;

    std::cout << "Welcome to JsonFileParser v1.0!" << std::endl << std::endl;

    while(true) {
        std::cin >> command;

        if(command == "open") {
            OpenCommand openCommand;
            openCommand.execute();
        }
        else if(command == "close") {
            CloseCommand closeCommand;
            closeCommand.execute();
        }
        else if(command == "validate") {
            ValidateCommand validateCommand;
            validateCommand.execute();
        }
        else if(command == "print") {
            PrintCommand printCommand;
            printCommand.execute();
        }
        else if(command == "search") {
            SearchCommand searchCommand;
            searchCommand.execute();
        }
        else if(command == "set") {
            SetCommand setCommand;
            setCommand.execute();
        }
        else if(command == "create") {
            CreateCommand createCommand;
            createCommand.execute();
        }
        else if(command == "remove") {
            RemoveCommand removeCommand;
            removeCommand.execute();
        }
        else if(command == "move") {
            MoveCommand moveCommand;
            moveCommand.execute();
        }
        else if(command == "save") {
            SaveCommand saveCommand;
            saveCommand.execute();
        }
        else if(command == "save_as") {
            SaveAsCommand saveAsCommand;
            saveAsCommand.execute();
        }
        else if(command == "help") {
            HelpCommand helpCommand;
            helpCommand.execute();
        }
        else if(command == "exit") {
            ExitCommand exitCommand;
            exitCommand.execute();
            return;
        }
        else {
            std::cout << "Unknown command." << std::endl << std::endl;
        }
    }
}
