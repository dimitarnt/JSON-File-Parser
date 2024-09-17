#pragma once
#include "JsonCommand.h"

class MoveFromCommand : public JsonCommand {
private:
    String _pathFrom;

public:
    MoveFromCommand(const String& pathFrom);
    MoveFromCommand(String&& pathFrom);

    void execute() const override;
};
