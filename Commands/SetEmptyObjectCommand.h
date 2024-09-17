#pragma once
#include "JsonCommand.h"

class SetEmptyObjectCommand : public JsonCommand {
private:
    String _path;

public:
    explicit SetEmptyObjectCommand(const String& path);
    explicit SetEmptyObjectCommand(String&& path);

    void execute() const override;
};
