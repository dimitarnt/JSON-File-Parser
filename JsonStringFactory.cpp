#include "JsonStringFactory.h"
#include "JsonString.h"

SharedPtr<JsonNode> JsonStringFactory::create(std::ifstream& in) const {
    return new JsonString(in);
}

SharedPtr<JsonNode> JsonStringFactory::create(const String& value) {
    return new JsonString(value);
}

SharedPtr<JsonNode> JsonStringFactory::create(String&& value) {
    return new JsonString(std::move(value));
}

SharedPtr<JsonNode> JsonStringFactory::create(const char* value) {
    return new JsonString(value);
}
