#pragma once
#include "JsonCommand.h"

class RenameCommand : public JsonCommand {
private:
    String _newKey;
    String _path;

public:
    RenameCommand(const String& newKey, const String& path);
    RenameCommand(String&& newKey, String&& path);
    RenameCommand(const String& newKey, String&& path);
    RenameCommand(String&& newKey, const String& path);

    void execute() const override;
};
