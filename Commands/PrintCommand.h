#pragma once
#include "JsonCommand.h"

class PrintCommand : public JsonCommand {
public:
    PrintCommand() = default;
    void execute() const override;
};
