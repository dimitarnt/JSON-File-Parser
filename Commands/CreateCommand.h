#pragma once
#include "JsonCommand.h"

class CreateCommand : public JsonCommand {
public:
    CreateCommand() = default;
    void execute() const override;
};
