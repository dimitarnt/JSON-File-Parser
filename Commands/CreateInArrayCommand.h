#pragma once
#include "JsonCommand.h"
#include "SharedPtr.hpp"
#include "JsonNode.h"

class CreateInArrayCommand : public JsonCommand {
private:
    SharedPtr<JsonNode> _newNode;
    String _path;

public:
    CreateInArrayCommand(const SharedPtr<JsonNode>& newNode, const String& path);
    CreateInArrayCommand(SharedPtr<JsonNode>&& newNode, String&& path);
    CreateInArrayCommand(const SharedPtr<JsonNode>& newNode, String&& path);
    CreateInArrayCommand(SharedPtr<JsonNode>&& newNode, const String& path);

    void execute() const override;
};
