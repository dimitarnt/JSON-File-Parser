#pragma once
#include "JsonCommand.h"

class HelpCommand : public JsonCommand {
public:
    HelpCommand();
    void execute() const override;
};
