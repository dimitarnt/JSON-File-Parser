#pragma once
#include "JsonCommand.h"

class MoveToArrayCommand : public JsonCommand {
private:
    String _pathTo;
    String _pathFrom;

public:
    MoveToArrayCommand(const String& pathTo, const String& pathFrom);
    MoveToArrayCommand(String&& pathTo, String&& pathFrom);
    MoveToArrayCommand(const String& pathTo, String&& pathFrom);
    MoveToArrayCommand(String&& pathTo, const String& pathFrom);

    void execute() const override;
};
