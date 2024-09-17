#pragma once
#include "JsonCommand.h"

class CloseCommand : public JsonCommand {
public:
    CloseCommand();
    void execute() const override;
};
