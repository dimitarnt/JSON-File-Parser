#pragma once
#include "JsonCommand.h"

class CreateEmptyArrayInArrayCommand : public JsonCommand {
private:
    String _path;

public:
    explicit CreateEmptyArrayInArrayCommand(const String& path);
    explicit CreateEmptyArrayInArrayCommand(String&& path);

    void execute() const override;
};
