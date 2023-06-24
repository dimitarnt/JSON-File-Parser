#pragma once
#include "JsonCommand.h"

class CloseCommand : public JsonCommand {
public:
    CloseCommand() = default;
    void execute() const override;
};
