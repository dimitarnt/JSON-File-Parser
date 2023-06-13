#pragma once
#include "String.h"
#include "JsonNodeFactory.h"

class JsonStringFactory : public JsonNodeFactory {
public:
    JsonNode* create(std::ifstream& in) const override;

    static JsonNode* create(const String& value);
    static JsonNode* create(String&& value);
    static JsonNode* create(const char* value);
};
