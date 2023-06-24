#pragma once
#include "JsonCommand.h"

class SetCommand : public JsonCommand {
public:
    SetCommand() = default;
    void execute() const override;
};
