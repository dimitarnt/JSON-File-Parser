#include "JsonStringFactory.h"
#include "JsonString.h"

JsonNode* JsonStringFactory::create(std::ifstream& in) const {
    return new JsonString(in);
}

JsonNode* JsonStringFactory::create(const String& value) {
    return new JsonString(value);
}

JsonNode* JsonStringFactory::create(String&& value) {
    return new JsonString(std::move(value));
}

JsonNode* JsonStringFactory::create(const char* value) {
    return new JsonString(value);
}
