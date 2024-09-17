#pragma once
#include "JsonCommand.h"
#include "SharedPtr.hpp"
#include "JsonNode.h"

class SetCommand : public JsonCommand {
private:
    SharedPtr<JsonNode> _newNode;
    String _path;

public:
    SetCommand(const SharedPtr<JsonNode>& newNode, const String& path);
    SetCommand(SharedPtr<JsonNode>&& newNode, String&& path);
    SetCommand(const SharedPtr<JsonNode>& newNode, String&& path);
    SetCommand(SharedPtr<JsonNode>&& newNode, const String& path);

    void execute() const override;
};
