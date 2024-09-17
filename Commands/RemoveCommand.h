#pragma once
#include "JsonCommand.h"

class RemoveCommand : public JsonCommand {
private:
    String _path;

public:
    explicit RemoveCommand(const String& path);
    explicit RemoveCommand(String&& path);

    void execute() const override;
};
