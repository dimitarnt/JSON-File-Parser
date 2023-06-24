#pragma once
#include "JsonCommand.h"

class RemoveCommand : public JsonCommand {
public:
    RemoveCommand() = default;
    void execute() const override;
};
