#pragma once
#include <fstream>
#include "SharedPtr.hpp"
#include "JsonNode.h"
#include "String.h"

//JsonNode factory class
class JsonNodeFactory {
public:
    static SharedPtr<JsonNode> create(std::ifstream& in, JsonNodeType type);

    static SharedPtr<JsonNode> createEmptyJsonObject();
    static SharedPtr<JsonNode> createEmptyJsonArray();

    static SharedPtr<JsonNode> createJsonString(const String& str);
    static SharedPtr<JsonNode> createJsonString(String&& str);

    static SharedPtr<JsonNode> createJsonKeyword(const String& keyword);
    static SharedPtr<JsonNode> createJsonKeyword(String&& keyword);

    static SharedPtr<JsonNode> createJsonNumber(const String& number);
    static SharedPtr<JsonNode> createJsonNumber(String&& number);
};
