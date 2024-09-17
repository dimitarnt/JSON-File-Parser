#pragma once
#include "SharedPtr.hpp"
#include "Vector.hpp"
#include "JsonCommand.h"

class JsonCommandFactory {
private:
    static Vector<String> parseCommandParameters(String&& input, unsigned numberOfParameters, unsigned numberOfOptionalLastParameters);
    static String getInputWithoutCommandString(String&& input);

    static SharedPtr<JsonCommand> createOpenCommand(String&& input);
    static SharedPtr<JsonCommand> createCloseCommand(String&& input);
    static SharedPtr<JsonCommand> createValidateCommand(String&& input);
    static SharedPtr<JsonCommand> createPrintCommand(String&& input);
    static SharedPtr<JsonCommand> createSearchCommand(String&& input);
    static SharedPtr<JsonCommand> createTraverseCommand(String&& input);
    static SharedPtr<JsonCommand> createPreviousCommand(String&& input);
    static SharedPtr<JsonCommand> createGoBackCommand(String&& input);
    static SharedPtr<JsonCommand> createUndoCommand(String&& input);
    static SharedPtr<JsonCommand> createUndoAllCommand(String&& input);
    static SharedPtr<JsonCommand> createRenameCommand(String&& input);
    static SharedPtr<JsonCommand> createSetEmptyObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createSetEmptyArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createSetStringCommand(String&& input);
    static SharedPtr<JsonCommand> createSetKeywordCommand(String&& input);
    static SharedPtr<JsonCommand> createSetNumberCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateEmptyObjectInArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateEmptyObjectInObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateEmptyArrayInArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateEmptyArrayInObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateStringInArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateStringInObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateKeywordInArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateKeywordInObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateNumberInArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createCreateNumberInObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createRemoveCommand(String&& input);
    static SharedPtr<JsonCommand> createMoveFromCommand(String&& input);
    static SharedPtr<JsonCommand> createMoveToArrayCommand(String&& input);
    static SharedPtr<JsonCommand> createMoveToObjectCommand(String&& input);
    static SharedPtr<JsonCommand> createSaveCommand(String&& input);
    static SharedPtr<JsonCommand> createSaveAsCommand(String&& input);
    static SharedPtr<JsonCommand> createHelpCommand(String&& input);
    static SharedPtr<JsonCommand> createExitCommand(String&& input);

public:
    static SharedPtr<JsonCommand> getCommand();
};
