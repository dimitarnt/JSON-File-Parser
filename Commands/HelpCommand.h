#pragma once
#include "JsonCommand.h"

class HelpCommand : public JsonCommand {
public:
    HelpCommand() = default;
    void execute() const override;
};
