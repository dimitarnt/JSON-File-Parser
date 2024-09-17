#include "JsonNodeFactory.h"
#include "JsonObject.h"
#include "JsonArray.h"
#include "JsonString.h"
#include "JsonKeyword.h"
#include "JsonNumber.h"

SharedPtr<JsonNode> JsonNodeFactory::create(std::ifstream& in, JsonNodeType type) {

    switch(type) {
        case JsonNodeType::JSON_OBJECT:
            return new JsonObject(in);

        case JsonNodeType::JSON_ARRAY:
            return new JsonArray(in);

        case JsonNodeType::JSON_STRING:
            return new JsonString(in);

        case JsonNodeType::JSON_KEYWORD:
            return new JsonKeyword(in);

        case JsonNodeType::JSON_NUMBER:
            return new JsonNumber(in);

        default:
            return nullptr;
    }
}

SharedPtr<JsonNode> JsonNodeFactory::createEmptyJsonObject() {
    return new JsonObject();
}

SharedPtr<JsonNode> JsonNodeFactory::createEmptyJsonArray() {
    return new JsonArray();
}

SharedPtr<JsonNode> JsonNodeFactory::createJsonString(const String& str) {
    return new JsonString(str);
}

SharedPtr<JsonNode> JsonNodeFactory::createJsonString(String&& str) {
    return new JsonString(std::move(str));
}

SharedPtr<JsonNode> JsonNodeFactory::createJsonKeyword(const String& keyword) {
    return new JsonKeyword(keyword);
}

SharedPtr<JsonNode> JsonNodeFactory::createJsonKeyword(String&& keyword) {
    return new JsonKeyword(std::move(keyword));
}

SharedPtr<JsonNode> JsonNodeFactory::createJsonNumber(const String& number) {
    return new JsonNumber(number);
}

SharedPtr<JsonNode> JsonNodeFactory::createJsonNumber(String&& number) {
    return new JsonNumber(std::move(number));
}
