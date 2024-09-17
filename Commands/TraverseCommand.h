#pragma once
#include "JsonCommand.h"

class TraverseCommand : public JsonCommand {
private:
    String _path;

public:
    explicit TraverseCommand(const String& path);
    explicit TraverseCommand(String&& path);

    void execute() const override;
};
