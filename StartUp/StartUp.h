#pragma once

class StartUp {
private:
    static void saveChangesPrompt();
    static void runOpenCommand();
    static void runCloseCommand();
    static void runValidateCommand();
    static void runPrintCommand();
    static void runSearchCommand();
    static void runSetCommand();
    static void runCreateCommand();
    static void runRemoveCommand();
    static void runMoveCommand();
    static void runSaveCommand();
    static void runSaveAsCommand();
    static void runHelpCommand();
    static void runExitCommand();

public:
    static void run();
};
