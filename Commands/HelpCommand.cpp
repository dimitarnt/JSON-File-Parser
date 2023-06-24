#include "HelpCommand.h"
#include "String.h"

void HelpCommand::execute() const {
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
