#pragma once
#include "JsonCommand.h"

class SaveCommand : public JsonCommand {
public:
    SaveCommand() = default;
    void execute() const override;
};
