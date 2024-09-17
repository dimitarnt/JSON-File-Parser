#pragma once
#include "JsonCommand.h"

class ValidateCommand : public JsonCommand {
private:
    String _fileName;

public:
    explicit ValidateCommand(const String& fileName);
    explicit ValidateCommand(String&& fileName);

    void execute() const override;
};
