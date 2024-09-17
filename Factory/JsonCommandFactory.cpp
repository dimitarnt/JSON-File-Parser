#include "JsonCommandFactory.h"
#include "OpenCommand.h"
#include "CloseCommand.h"
#include "ValidateCommand.h"
#include "PrintCommand.h"
#include "SearchCommand.h"
#include "TraverseCommand.h"
#include "PreviousCommand.h"
#include "GoBackCommand.h"
#include "UndoCommand.h"
#include "UndoAllCommand.h"
#include "RenameCommand.h"
#include "SetEmptyObjectCommand.h"
#include "SetEmptyArrayCommand.h"
#include "SetStringCommand.h"
#include "SetKeywordCommand.h"
#include "SetNumberCommand.h"
#include "CreateEmptyObjectInArrayCommand.h"
#include "CreateEmptyObjectInObjectCommand.h"
#include "CreateEmptyArrayInArrayCommand.h"
#include "CreateEmptyArrayInObjectCommand.h"
#include "CreateStringInArrayCommand.h"
#include "CreateStringInObjectCommand.h"
#include "CreateKeywordInArrayCommand.h"
#include "CreateKeywordInObjectCommand.h"
#include "CreateNumberInArrayCommand.h"
#include "CreateNumberInObjectCommand.h"
#include "RemoveCommand.h"
#include "MoveFromCommand.h"
#include "MoveToArrayCommand.h"
#include "MoveToObjectCommand.h"
#include "SaveCommand.h"
#include "SaveAsCommand.h"
#include "HelpCommand.h"
#include "ExitCommand.h"

namespace {
    const String OPEN_COMMAND = "open";
    const String CLOSE_COMMAND = "close";
    const String VALIDATE_COMMAND = "validate";
    const String PRINT_COMMAND = "print";
    const String SEARCH_COMMAND = "search";
    const String TRAVERSE_COMMAND = "traverse";
    const String PREVIOUS_COMMAND = "previous";
    const String GO_BACK_COMMAND = "go_back";
    const String UNDO_COMMAND = "undo";
    const String UNDO_ALL_COMMAND = "undo_all";
    const String RENAME_COMMAND = "rename";
    const String SET_EMPTY_OBJECT_COMMAND = "set_obj";
    const String SET_EMPTY_ARRAY_COMMAND = "set_arr";
    const String SET_STRING_COMMAND = "set_str";
    const String SET_KEYWORD_COMMAND = "set_keyword";
    const String SET_NUMBER_COMMAND = "set_num";
    const String CREATE_EMPTY_OBJECT_IN_ARRAY_COMMAND = "create_obj_in_arr";
    const String CREATE_EMPTY_OBJECT_IN_OBJECT_COMMAND = "create_obj_in_obj";
    const String CREATE_EMPTY_ARRAY_IN_ARRAY_COMMAND = "create_arr_in_arr";
    const String CREATE_EMPTY_ARRAY_IN_OBJECT_COMMAND = "create_arr_in_obj";
    const String CREATE_STRING_IN_ARRAY_COMMAND = "create_str_in_arr";
    const String CREATE_STRING_IN_OBJECT_COMMAND = "create_str_in_obj";
    const String CREATE_KEYWORD_IN_ARRAY_COMMAND = "create_keyword_in_arr";
    const String CREATE_KEYWORD_IN_OBJECT_COMMAND = "create_keyword_in_obj";
    const String CREATE_NUMBER_IN_ARRAY_COMMAND = "create_num_in_arr";
    const String CREATE_NUMBER_IN_OBJECT_COMMAND = "create_num_in_obj";
    const String REMOVE_COMMAND = "remove";
    const String MOVE_FROM_COMMAND = "move_from";
    const String MOVE_TO_ARRAY_COMMAND = "move_to_arr";
    const String MOVE_TO_OBJECT_COMMAND = "move_to_obj";
    const String SAVE_COMMAND = "save";
    const String SAVE_AS_COMMAND = "save_as";
    const String HELP_COMMAND = "help";
    const String EXIT_COMMAND = "exit";
}

SharedPtr<JsonCommand> JsonCommandFactory::getCommand() {
    String input;
    String command;

    std::cin >> input;
    command = input.getFormattedWord(1);

    if(command == OPEN_COMMAND) {
        return createOpenCommand(std::move(input));
    }
    else if(command == CLOSE_COMMAND) {
        return createCloseCommand(std::move(input));
    }
    else if(command == VALIDATE_COMMAND) {
        return createValidateCommand(std::move(input));
    }
    else if(command == PRINT_COMMAND) {
        return createPrintCommand(std::move(input));
    }
    else if(command == SEARCH_COMMAND) {
        return createSearchCommand(std::move(input));
    }
    else if(command == TRAVERSE_COMMAND) {
        return createTraverseCommand(std::move(input));
    }
    else if(command == PREVIOUS_COMMAND) {
        return createPreviousCommand(std::move(input));
    }
    else if(command == GO_BACK_COMMAND) {
        return createGoBackCommand(std::move(input));
    }
    else if(command == UNDO_COMMAND) {
        return createUndoCommand(std::move(input));
    }
    else if(command == UNDO_ALL_COMMAND) {
        return createUndoAllCommand(std::move(input));
    }
    else if(command == RENAME_COMMAND) {
        return createRenameCommand(std::move(input));
    }
    else if(command == SET_EMPTY_OBJECT_COMMAND) {
        return createSetEmptyObjectCommand(std::move(input));
    }
    else if(command == SET_EMPTY_ARRAY_COMMAND) {
        return createSetEmptyArrayCommand(std::move(input));
    }
    else if(command == SET_STRING_COMMAND) {
        return createSetStringCommand(std::move(input));
    }
    else if(command == SET_KEYWORD_COMMAND) {
        return createSetKeywordCommand(std::move(input));
    }
    else if(command == SET_NUMBER_COMMAND) {
        return createSetNumberCommand(std::move(input));
    }
    else if(command == CREATE_EMPTY_OBJECT_IN_ARRAY_COMMAND) {
        return createCreateEmptyObjectInArrayCommand(std::move(input));
    }
    else if(command == CREATE_EMPTY_OBJECT_IN_OBJECT_COMMAND) {
        return createCreateEmptyObjectInObjectCommand(std::move(input));
    }
    else if(command == CREATE_EMPTY_ARRAY_IN_ARRAY_COMMAND) {
        return createCreateEmptyArrayInArrayCommand(std::move(input));
    }
    else if(command == CREATE_EMPTY_ARRAY_IN_OBJECT_COMMAND) {
        return createCreateEmptyArrayInObjectCommand(std::move(input));
    }
    else if(command == CREATE_STRING_IN_ARRAY_COMMAND) {
        return createCreateStringInArrayCommand(std::move(input));
    }
    else if(command == CREATE_STRING_IN_OBJECT_COMMAND) {
        return createCreateStringInObjectCommand(std::move(input));
    }
    else if(command == CREATE_KEYWORD_IN_ARRAY_COMMAND) {
        return createCreateKeywordInArrayCommand(std::move(input));
    }
    else if(command == CREATE_KEYWORD_IN_OBJECT_COMMAND) {
        return createCreateKeywordInObjectCommand(std::move(input));
    }
    else if(command == CREATE_NUMBER_IN_ARRAY_COMMAND) {
        return createCreateNumberInArrayCommand(std::move(input));
    }
    else if(command == CREATE_NUMBER_IN_OBJECT_COMMAND) {
        return createCreateNumberInObjectCommand(std::move(input));
    }
    else if(command == REMOVE_COMMAND) {
        return createRemoveCommand(std::move(input));
    }
    else if(command == MOVE_FROM_COMMAND) {
        return createMoveFromCommand(std::move(input));
    }
    else if(command == MOVE_TO_ARRAY_COMMAND) {
        return createMoveToArrayCommand(std::move(input));
    }
    else if(command == MOVE_TO_OBJECT_COMMAND) {
        return createMoveToObjectCommand(std::move(input));
    }
    else if(command == SAVE_COMMAND) {
        return createSaveCommand(std::move(input));
    }
    else if(command == SAVE_AS_COMMAND) {
        return createSaveAsCommand(std::move(input));
    }
    else if(command == HELP_COMMAND) {
        return createHelpCommand(std::move(input));
    }
    else if(command == EXIT_COMMAND) {
        return createExitCommand(std::move(input));
    }

    throw std::invalid_argument("Unknown command");
}

String JsonCommandFactory::getInputWithoutCommandString(String&& input) {
    size_t inputLength = input.getLength();
    bool commandHasStarted = false;

    for(size_t i = 0; i <= inputLength; ++i) {
        if(!commandHasStarted && (i == inputLength || input[i] == ' ' || input[i] == '\n' || input[i] == '\t')) {
            continue;
        }
        commandHasStarted = true;

        if(i == inputLength || input[i] == ' ' || input[i] == '\n' || input[i] == '\t') {
            return input.substring(i, input.getLength() - i);
        }
    }

    return "";
}

Vector<String> JsonCommandFactory::parseCommandParameters(String&& input, unsigned numberOfParameters,
                                                          unsigned numberOfOptionalLastParameters) {
    Vector<String> result;
    String inputWithoutCommandString = getInputWithoutCommandString(std::move(input));
    unsigned currentParameterIndex = -1;
    bool commandParameterHasStarted = false;

    for(size_t i = 0; i < inputWithoutCommandString.getLength(); ++i) {
        if(!commandParameterHasStarted && inputWithoutCommandString[i] == ' ') {
            continue;
        }
        else if(!commandParameterHasStarted && inputWithoutCommandString[i] == COMMAND_PARAMETER_STARTING_DELIMITER) {
            commandParameterHasStarted = true;
            currentParameterIndex++;

            if(currentParameterIndex >= numberOfParameters) {
                throw std::invalid_argument("Invalid command");
            }

            result.pushBack("");

            continue;
        }
        else if(!commandParameterHasStarted) {
            throw std::invalid_argument("Invalid command");
        }

        if(inputWithoutCommandString[i] == COMMAND_PARAMETER_ENDING_DELIMITER) {
            commandParameterHasStarted = false;
            continue;
        }

        result[currentParameterIndex] += inputWithoutCommandString[i];
    }

    if(commandParameterHasStarted) {
        throw std::invalid_argument("Invalid command");
    }

    if(currentParameterIndex + 1 == numberOfParameters) {
        return result;
    }

    for(unsigned i = 1; i <= numberOfOptionalLastParameters; ++i) {

        if(currentParameterIndex + i + 1 == numberOfParameters) {

            for(unsigned j = 0; j < i; ++j) {
                result.pushBack("");
            }

            return result;
        }
    }

    throw std::invalid_argument("Invalid command");
}

SharedPtr<JsonCommand> JsonCommandFactory::createOpenCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 0);

    return new OpenCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCloseCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new CloseCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createValidateCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 0);

    return new ValidateCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createPrintCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new PrintCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createSearchCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 0);

    return new SearchCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createTraverseCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 1);

    return new TraverseCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createPreviousCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new PreviousCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createGoBackCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new GoBackCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createUndoCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new UndoCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createUndoAllCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new UndoAllCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createRenameCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 0);

    return new RenameCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSetEmptyObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 1);

    return new SetEmptyObjectCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSetEmptyArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 1);

    return new SetEmptyArrayCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSetStringCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 1);

    return new SetStringCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSetKeywordCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 1);

    return new SetKeywordCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSetNumberCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 1);

    return new SetNumberCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateEmptyObjectInArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 0);

    return new CreateEmptyObjectInArrayCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateEmptyObjectInObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 1);

    return new CreateEmptyObjectInObjectCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateEmptyArrayInArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 0);

    return new CreateEmptyArrayInArrayCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateEmptyArrayInObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 1);

    return new CreateEmptyArrayInObjectCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateStringInArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 0);

    return new CreateStringInArrayCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateStringInObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 3, 1);

    return new CreateStringInObjectCommand(std::move(commandParameters[0]), std::move(commandParameters[1]), std::move(commandParameters[2]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateKeywordInArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 0);

    return new CreateKeywordInArrayCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateKeywordInObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 3, 1);

    return new CreateKeywordInObjectCommand(std::move(commandParameters[0]), std::move(commandParameters[1]), std::move(commandParameters[2]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateNumberInArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 0);

    return new CreateNumberInArrayCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createCreateNumberInObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 3, 1);

    return new CreateNumberInObjectCommand(std::move(commandParameters[0]), std::move(commandParameters[1]), std::move(commandParameters[2]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createRemoveCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 0);

    return new RemoveCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createMoveFromCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 1);

    return new MoveFromCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createMoveToArrayCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 2);

    return new MoveToArrayCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createMoveToObjectCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 2);

    return new MoveToObjectCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSaveCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 1, 1);

    return new SaveCommand(std::move(commandParameters[0]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createSaveAsCommand(String&& input) {
    Vector<String> commandParameters = parseCommandParameters(std::move(input), 2, 1);

    return new SaveAsCommand(std::move(commandParameters[0]), std::move(commandParameters[1]));
}

SharedPtr<JsonCommand> JsonCommandFactory::createHelpCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new HelpCommand();
}

SharedPtr<JsonCommand> JsonCommandFactory::createExitCommand(String&& input) {
    if(input.getFormattedWord(2) != "") {
        throw std::invalid_argument("Invalid command");
    }

    return new ExitCommand();
}
