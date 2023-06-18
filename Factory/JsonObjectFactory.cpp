#include "JsonObjectFactory.h"
#include "JsonObject.h"

SharedPtr<JsonNode> JsonObjectFactory::create(std::ifstream& in) const {
    return new JsonObject(in);
}
