#pragma once
#include "JsonCommand.h"

class ExitCommand : public JsonCommand {
public:
    ExitCommand();
    void execute() const override;
};
