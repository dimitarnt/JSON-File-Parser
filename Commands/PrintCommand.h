#pragma once
#include "JsonCommand.h"

class PrintCommand : public JsonCommand {
public:
    PrintCommand();
    void execute() const override;
};
