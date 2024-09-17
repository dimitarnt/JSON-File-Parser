#pragma once
#include "JsonCommand.h"

class UndoAllCommand : public JsonCommand {
public:
    UndoAllCommand();
    void execute() const override;
};
