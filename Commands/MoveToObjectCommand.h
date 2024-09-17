#pragma once
#include "JsonCommand.h"

class MoveToObjectCommand : public JsonCommand {
private:
    String _pathTo;
    String _pathFrom;
    String _oldKey;

public:
    MoveToObjectCommand(const String& pathTo, const String& pathFrom);
    MoveToObjectCommand(String&& pathTo, String&& pathFrom);
    MoveToObjectCommand(const String& pathTo, String&& pathFrom);
    MoveToObjectCommand(String&& pathTo, const String& pathFrom);

    MoveToObjectCommand(const String& pathTo, const String& pathFrom, const String& oldKey);
    MoveToObjectCommand(const String& pathTo, const String& pathFrom, String&& oldKey);
    MoveToObjectCommand(const String& pathTo, String&& pathFrom, String&& oldKey);
    MoveToObjectCommand(String&& pathTo, const String& pathFrom, String&& oldKey);
    MoveToObjectCommand(String&& pathTo, String&& pathFrom, const String& oldKey);
    MoveToObjectCommand(String&& pathTo, const String& pathFrom, const String& oldKey);
    MoveToObjectCommand(const String& pathTo, String&& pathFrom, const String& oldKey);
    MoveToObjectCommand(String&& pathTo, String&& pathFrom, String&& oldKey);

    void execute() const override;
};
