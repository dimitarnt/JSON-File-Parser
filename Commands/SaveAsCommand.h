#pragma once
#include "JsonCommand.h"

class SaveAsCommand : public JsonCommand {
public:
    SaveAsCommand() = default;
    void execute() const override;
};
