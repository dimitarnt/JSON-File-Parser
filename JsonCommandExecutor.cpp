#include "JsonCommandExecutor.h"
#include "JsonCommandFactory.h"

void JsonCommandExecutor::run() {

    while(true) {
        try {
            SharedPtr<JsonCommand> currentCommand = JsonCommandFactory::getCommand();
            currentCommand->execute();

            if(currentCommand->getType() == JsonCommandType::EXIT_COMMAND) {
                break;
            }
        }
        catch(const std::exception& exception) {
            std::cout << exception.what() << std::endl << std::endl;
        }
    }
}
