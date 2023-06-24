#pragma once
#include "JsonCommand.h"

class ExitCommand : public JsonCommand {
public:
    ExitCommand() = default;
    void execute() const override;
};
