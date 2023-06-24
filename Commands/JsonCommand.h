#pragma once

class JsonCommand {
public:
    virtual ~JsonCommand() = default;
    virtual void execute() const = 0;

    static void saveChangesPrompt();
};
