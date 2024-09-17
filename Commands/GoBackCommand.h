#pragma once
#include "JsonCommand.h"

class GoBackCommand : public JsonCommand {
public:
    GoBackCommand();
    void execute() const override;
};
