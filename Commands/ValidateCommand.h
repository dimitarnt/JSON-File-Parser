#pragma once
#include "JsonCommand.h"

class ValidateCommand : public JsonCommand {
public:
    ValidateCommand() = default;
    void execute() const override;
};
