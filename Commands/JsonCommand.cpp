#include "JsonCommand.h"
#include "String.h"
#include "JsonParser.h"

void JsonCommand::saveChangesPrompt() {
    String answer;

    if(JsonParser::getInstance()->fileIsOpened() && JsonParser::getInstance()->changesHaveBeenMade()) {
        std::cout << "Would you like to save changes?(yes/no)" << std::endl;
        std::cin >> answer;

        if(answer == "yes") {

            try {
                JsonParser::getInstance()->save();
            }
            catch(const std::exception& exception) {
                std::cout << exception.what() << std::endl << std::endl;
                return;
            }
        }
        else if(answer != "no") {
            std::cout << "Invalid answer." << std::endl << std::endl;
            return;
        }
    }
}
