#pragma once
#include "JsonCommand.h"

class UndoCommand : public JsonCommand {
public:
    UndoCommand();
    void execute() const override;
};
