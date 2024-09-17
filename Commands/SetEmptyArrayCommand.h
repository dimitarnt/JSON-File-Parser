#pragma once
#include "JsonCommand.h"

class SetEmptyArrayCommand : public JsonCommand {
private:
    String _path;

public:
    explicit SetEmptyArrayCommand(const String& path);
    explicit SetEmptyArrayCommand(String&& path);

    void execute() const override;
};
