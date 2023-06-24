#pragma once
#include "JsonCommand.h"

class OpenCommand : public JsonCommand {
public:
    OpenCommand() = default;
    void execute() const override;
};
