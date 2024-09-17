#pragma once
#include "JsonCommand.h"

class SaveCommand : public JsonCommand {
private:
    String _path;

public:
    explicit SaveCommand(const String& path);
    explicit SaveCommand(String&& path);

    void execute() const override;
};
