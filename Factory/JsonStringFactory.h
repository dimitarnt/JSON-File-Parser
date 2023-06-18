#pragma once
#include "String.h"
#include "JsonNodeFactory.h"

class JsonStringFactory : public JsonNodeFactory {
public:
    SharedPtr<JsonNode> create(std::ifstream& in) const override;

    static SharedPtr<JsonNode> create(const String& value);
    static SharedPtr<JsonNode> create(String&& value);
    static SharedPtr<JsonNode> create(const char* value);
};
