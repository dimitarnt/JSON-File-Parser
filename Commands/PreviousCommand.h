#pragma once
#include "JsonCommand.h"

class PreviousCommand : public JsonCommand {
public:
    PreviousCommand();
    void execute() const override;
};
