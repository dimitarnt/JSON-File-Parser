#pragma once
#include "JsonCommand.h"

class MoveCommand : public JsonCommand {
public:
    MoveCommand() = default;
    void execute() const override;
};
