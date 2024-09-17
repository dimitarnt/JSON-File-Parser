#pragma once
#include "JsonCommand.h"

class OpenCommand : public JsonCommand {
private:
    String _fileName;

public:
    explicit OpenCommand(const String& fileName);
    explicit OpenCommand(String&& fileName);

    void execute() const override;
};
