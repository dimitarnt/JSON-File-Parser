#pragma once
#include "JsonCommand.h"

class CreateEmptyObjectInArrayCommand : public JsonCommand {
private:
    String _path;

public:
    explicit CreateEmptyObjectInArrayCommand(const String& path);
    explicit CreateEmptyObjectInArrayCommand(String&& path);

    void execute() const override;
};
